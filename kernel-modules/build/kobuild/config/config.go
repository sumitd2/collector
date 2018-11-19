package config

import (
	"fmt"
	"io/ioutil"
	"sort"
	"strings"

	"github.com/kballard/go-shellquote"
	"gopkg.in/yaml.v2"
)

// Builders represents a named map of Builder instances.
type Builders map[string]Builder

// Builder represents a single builder configuration. This captures all kernel
// versions that can be produced by the given type.
type Builder struct {
	Description string                         `yaml:"description"`
	Kind        string                         `yaml:"type"`
	Versions    map[string]map[string][]string `yaml:"versions"`
}

// Manifest represents a fully self-contained kernel build unit. All
// information required for building a single kernel module is captured in a
// Manifest instance.
type Manifest struct {
	Builder     string
	Description string
	Version     string
	Packages    []string
	Kind        string
	Flavor      string
	Build       bool
}

// Fullname returns a unique name for the current manifest. This name is
// intended to be used for lexicographically sorting multiple manifests.
func (m *Manifest) Fullname() string {
	return fmt.Sprintf("%s-%s-%s", m.Builder, m.Flavor, m.Version)
}

// KernelVersion returns the kernel version for the given manifest to match
// the output of `uname -r`.
func (m *Manifest) KernelVersion() string {
	sep := "-"
	if m.Kind == "RedHat" {
		sep = "."
	}
	return fmt.Sprintf("%s%s%s", m.Version, sep, m.Flavor)
}

// Load reads the given filename as yaml and parses the content into a list of
// Builders.
func Load(filename string) (Builders, error) {
	body, err := ioutil.ReadFile(filename)
	if err != nil {
		return nil, err
	}

	var builders map[string]Builder
	if err := yaml.UnmarshalStrict(body, &builders); err != nil {
		return nil, err
	}

	return builders, nil
}

// Manifests iterates across the given Builders and returns a consistently
// ordered list of build manifests as a cross-product.
func (b *Builders) Manifests() []*Manifest {
	var manifests = make([]*Manifest, 0, 512)

	for name, builder := range *b {
		for flavor, versions := range builder.Versions {
			for version, packages := range versions {
				manifest := Manifest{
					Builder:     name,
					Description: builder.Description,
					Kind:        builder.Kind,
					Version:     version,
					Flavor:      flavor,
					Packages:    packages,
					Build:       false,
				}
				manifests = append(manifests, &manifest)
			}
		}
	}

	sort.SliceStable(manifests, func(i, j int) bool {
		return manifests[i].Fullname() < manifests[j].Fullname()
	})

	return manifests
}

// BuildArgs returns the arguments passed to the ko builder as a string slice.
func (m *Manifest) BuildArgs() []string {
	args := []string{
		m.Kind, m.Version, m.Flavor,
	}
	args = append(args, m.Packages...)
	return args
}

// BuildCommand returns the shell-escaped build command for the ko build, using the given
// builder (base) command.
func (m *Manifest) BuildCommand(cmdName string) string {
	args := m.BuildArgs()
	allArgs := make([]string, len(args) + 1)
	allArgs[0] = cmdName
	copy(allArgs[1:], args)
	return shellquote.Join(allArgs...)
}

// PackageList returns the newline-separted list of packages required for the build as a
// single string.
func (m *Manifest) PackageList() string {
	return strings.Join(m.Packages, "\n")
}
