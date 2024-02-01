# SparkFun Toolkit Arduino Library

![Toolkit Tests Builds](https://github.com/sparkfun/SparkFun_Toolkit/actions/workflows/compile-sketch.yml/badge.svg)
![GitHub issues](https://img.shields.io/github/issues/sparkfun/SparkFun_Toolkit)
![GitHub release (with filter)](https://img.shields.io/github/v/release/sparkfun/SparkFun_Toolkit)
![GitHub (Pre-)Release Date](https://img.shields.io/github/release-date-pre/sparkfun/SparkFun_Toolkit)



The SparkFun Toolkit provides a common set of core functionality for use across the SparkFun Arduino Driver library. Instead of each device driver library implementing it's own communication layers, error types and design, the SparkFun Toolkit library is used.

By using the SparkFun Toolkit, Arduino drivers achieve the following benefits:

* Use a well-tested and validated implementation
* Reduce development effort
* Implement functionality following a common structure
* Set the foundation for future enhancements - as the capabilities of the toolkit grow, these features become available with little to any implementation effort.

## Current Status

### December 2023

The SparkFun Toolkit is available as a *Beta* release, with the intent of testing and validation by SparkFun. The community are free to use this toolkit with the understanding that interfaces, types and class structures could change.

### Documentation

|||
|---|---|
|[Bus Implementation](docs/ar_ibus.md) | The architecture and use of the Toolkit Communication Bus Interface
