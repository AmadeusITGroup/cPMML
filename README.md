![cPMML](./docsrc/img/logo.png)

![Travis (.org)](https://img.shields.io/travis/AmadeusITGroup/cPMML?style=flat-square)
![GitHub](https://img.shields.io/github/license/AmadeusITGroup/cPMML?style=flat-square)
![](https://img.shields.io/badge/STD-C%2B%2B11-blue?style=flat-square)

# High-Performance PMML Scoring

*cPMML* is C++ library for scoring machine learning models serialized with the Predictive Model Markup Language ([PMML](http://dmg.org/pmml/v4-4/GeneralStructure.html)).
It exposes a minimalist and user-friendly API and it targets high performance in model scoring, keeping a predictable and minimal memory footprint.

Currently, the following PMML elements are supported:
* PMML General structure (preprocessing, data dictionary, etc.)
* Tree-based models
* Regression models
* Ensembles of the previous

## Getting Started

```cpp
#include "cPMML.h"

cpmml::Model model("IrisTree.xml");
std::unordered_map<std::string, std::string> sample = {
	{"sepal_length","6.6"},
	{"sepal_width","2.9"},
	{"petal_length","4.6"},
	{"petal_width","1.3"}
};

std::cout << model.predict(sample); // "Iris-versicolor"
```

## Set-up 
#### Linux / Mac
```
git clone https://github.com/AmadeusITGroup/cPMML.git && cd cPMML && ./install.sh
```
##### Prerequisites
* Git
* CMAKE >= 3.5.1
* Compiler supporting C++11


#### Windows
```
git clone https://github.com/AmadeusITGroup/cPMML.git && cd cPMML && install.bat
```
##### Prerequisites
* Git
* CMAKE >= 3.5.1
* MinGW-W64 supporting C++11

## Documentation
Please refer to the [official documentation](https://amadeusitgroup.github.io/cPMML/) for further details.

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on how to submit your pull requests.

## Authors

* **Paolo Iannino** - *Initial work* - [Paolo](https://github.com/piannino)

See also the list of [contributors](https://github.com/AmadeusITGroup/cPMML/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
