package main

import (
	"flag"
	"fmt"
	"os"
	"strconv"
	"text/template"

	"github.com/stackrox/collector/kernel-modules/build/kobuild/command"
	"github.com/stackrox/collector/kernel-modules/build/kobuild/config"
)

var (
	circleNodeTotal = getEnvVar("CIRCLE_NODE_TOTAL", 1)
	circleNodeIndex = getEnvVar("CIRCLE_NODE_INDEX", 0)
)

func log(format string, args ...interface{}) {
	fmt.Fprintf(os.Stderr, format, args...)
	fmt.Fprintln(os.Stderr)
}

func main() {
	if err := mainCmd(); err != nil {
		log("kobuild: %v", err)
		os.Exit(1)
	}
}

func mainCmd() error {
	configFlag := flag.String("config", "kernel-manifest.yml", "Config file containing build manifest")
	kernelVersionsFlag := flag.String("kernel-versions-file", "", "File containing kernel versions for which a module should be built")
	excludeFlag := flag.Bool("exclude", true, "If used in conjunction with -kernel-versions-file, treat the file as a list of excluded kernel versions")
	outputFlag := flag.String("output", "", "Go template string to use as output for each build")
	flag.Parse()

	builders, err := config.Load(*configFlag)
	if err != nil {
		return err
	}

	manifests := builders.Manifests()
	var kernelVersions map[string]struct{}
	if kernelVersionsFlag != nil && *kernelVersionsFlag != "" {
		kernelVersions, err = config.LoadKernelVersions(*kernelVersionsFlag)
		if err != nil {
			return err
		}
	}
	exclude := false
	if excludeFlag != nil {
		exclude = *excludeFlag
	}

	if kernelVersions == nil {
		markAllManifests(manifests)
	} else {
		markManifestsForKernelVersions(manifests, kernelVersions, exclude)
	}
	markManifestsForShardedBuild(manifests)

	manifestAction := buildManifest
	if outputFlag != nil {
		t := template.Must(template.New("").Parse(*outputFlag))
		manifestAction = createPrintAction(t)
	}

	return processManifests(manifests, manifestAction)
}

func markAllManifests(manifests []*config.Manifest) {
	for _, manifest := range manifests {
		manifest.Build = true
	}
}

func markManifestsForKernelVersions(manifests []*config.Manifest, kernelVersions map[string]struct{}, exclude bool) {
	for _, manifest := range manifests {
		_, exists := kernelVersions[manifest.KernelVersion()]
		manifest.Build = exclude != exists
	}
}

// markManifests examines each manifest and marks if a given manifest should be
// built on the current CircleCI node.
func markManifestsForShardedBuild(manifests []*config.Manifest) {
	i := 0
	for _, manifest := range manifests {
		if !manifest.Build {
			continue
		}
		if i%circleNodeTotal == circleNodeIndex {
			manifest.Build = true
		} else {
			manifest.Build = false
		}
		i++
	}
}

func printManifest(manifest *config.Manifest, t *template.Template) error {
	err := t.Execute(os.Stdout, manifest)
	fmt.Println()
	return err
}

func createPrintAction(t *template.Template) func(*config.Manifest) error {
	return func(manifest *config.Manifest) error {
		return printManifest(manifest, t)
	}
}

func buildManifest(manifest *config.Manifest) error {
	log("%s version %s-%s (%s)", manifest.Builder, manifest.Version, manifest.Flavor, manifest.Kind)
	log("  %s", manifest.Description)
	log("Files:")
	for _, pkg := range manifest.Packages {
		log("  - %s", pkg)
	}

	args := manifest.BuildArgs()
	log("Command:")
	log("  ./build-kos")
	for _, arg := range args {
		log("    %s", arg)
	}

	return command.Run("build-kos", args...)
}

func processManifests(manifests []*config.Manifest, action func(*config.Manifest) error) error {
	for index, manifest := range manifests {
		if manifest.Build == false {
			continue
		}

		log("Processing manifest %d/%d", index+1, len(manifests))
		if err := action(manifest); err != nil {
			log("Failed processing manifest %d/%d: %v", index+1, len(manifests), err)
			return err
		}

		log("Finished processing manifest %d/%d\n", index+1, len(manifests))
	}

	return nil
}

// getEnvVar looks up the variable named by key in the current environment and
// converts it into an integer. If the variable is not found or cannot be
// converted, the fallback value is returned instead.
func getEnvVar(key string, fallback int) int {
	value, found := os.LookupEnv(key)
	if !found {
		return fallback
	}

	parsed, err := strconv.Atoi(value)
	if err != nil {
		return fallback
	}

	return parsed
}
