************
PMML Support
************

Here are listed the PMML v4 elements currently supported by cPMML.

=================
General Structure
=================

- Header
- MiningBuildTask
- DataDictionary
- TransformationDictionary
- LocalTransformations
- MiningSchema
- Target
- Output

Note that Header and MiningBuildTask are ignored from cPMML
since they represents model metadata not useful for scoring.

==========
Data Types
==========

- STRING
- BOOLEAN
- INTEGER
- FLOAT
- DOUBLE

======
Models
======

- TreeModel
- RegressionModel
- MiningModel

==================
Built-in Functions
==================

- arithmetic operators

    - +, -, *, /, max, min, sum, avg

- boolean operators

    - =, !=, <, >, <=, >=, isMissing, isNotMissing, isIn, isNotIn

- string  modifiers (with REGEX support enabled, see Options)

    - replace, lowercase, uppercase, substr, trim_blanks, matches

===============
Transformations
===============

- Constant
- FieldRef
- NormContinous
- NormDiscrete
- Discretize
- MapValues
- Apply

=======================
Multiple Models Methods
=======================

- majorityVote
- weightedMajorityVote
- average
- weightedAverage
- sum
- modelChain

=======
Outputs
=======

- predictedValue
- predictedDisplayValue
- transformedValue
- probability
