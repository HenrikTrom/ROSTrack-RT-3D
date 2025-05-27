#!/bin/bash
set -e

# Unpack the tarball
tar -xf ./spinnaker-2.4.0.143-Ubuntu20.04-amd64-pkg.tar.gz

# Run the installer
cp ./install_spinnaker_auto.sh ./spinnaker-2.4.0.143-amd64/
cd ./spinnaker-2.4.0.143-amd64
sh ./install_spinnaker_auto.sh

# Set environment variables
echo "export PATH=/opt/spinnaker/lib:\$PATH" >> /root/.bashrc
echo "export LD_LIBRARY_PATH=/opt/spinnaker/lib:\$LD_LIBRARY_PATH" >> /root/.bashrc