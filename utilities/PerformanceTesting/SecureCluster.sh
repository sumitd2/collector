#!/usr/bin/env bash
set -eou pipefail

artifacts_dir=$1
collector_image_registry=${2:-docker.io/stackrox}
collector_image_tag=${3:-3.5.x-76-gaca574d047}

echo "Starting secure cluster services"

export KUBECONFIG=$artifacts_dir/kubeconfig

settings=(
    --set collector.roxAfterglowPeriod=10
    --set imagePullSecrets.username="$DOCKER_USERNAME"
    --set imagePullSecrets.password="$DOCKER_PASSWORD"
    --set clusterName=perf-test
    --set image.collector.registry="$collector_image_registry"
    --set image.collector.name=collector
    --set image.collector.tag="$collector_image_tag"
    --set enableOpenShiftMonitoring=true
    --set exposeMonitoring=true
)

if [[ -n ${IMAGE_MAIN_REGISTRY:-} ]]; then
    settings+=(--set image.main.registry="$IMAGE_MAIN_REGISTRY")
fi

if [[ -n ${IMAGE_MAIN_NAME:-} ]]; then
    settings+=(--set image.main.name="$IMAGE_MAIN_NAME")
fi

if [[ -n ${IMAGE_MAIN_TAG:-} ]]; then
    settings+=(--set image.main.tag="$IMAGE_MAIN_TAG")
fi

helm install -n stackrox stackrox-secured-cluster-services rhacs/secured-cluster-services \
    -f perf-bundle.yml \
    "${settings[@]}"