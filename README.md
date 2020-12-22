## Sandpolis Native Agent
_This instance module is a part of [Sandpolis](https://github.com/sandpolis/sandpolis)._

<p align="center">
	<img src="https://s3.us-east-2.amazonaws.com/github.sandpolis.com/agent/micro/snapshot.png" />
</p>

The *micro* agent is a lightweight/minimal alternative to the standard Java-based agent. It doesn't support plugins, but it's **small** and **fast**.

#### Motivation
The *micro* agent is designed for embedded systems with low amounts of memory and disk space. It provides barebones functionality in a non-portable package.

#### Supported Platforms
The *micro* agent is compatible with Linux, macOS, and Windows on a variety of CPU architectures.

#### Resource Requirements
The *micro* agent requires at least 2 MiB of storage space and 3 MiB of free memory.

#### UEFI Mode
Operations like cold snapshots require the device to be booted in a special environment. The micro agent can be installed to the system's ESP to achieve this.
