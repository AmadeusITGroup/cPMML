*********
Internals
*********

It follows the documentation related to the internals of *cPMML*. Here you can find all *cPMML* classes not exposed in the public API.

========================
cPMML Driving Principles
========================

Model Load and Model Scoring
----------------------------
*cPMML* design is split around two phases:

    Model Loading
        The XML content is loaded from disk to memory and the corresponding c++ object is built.
    Model Scoring
        The model object is evaluated against an user provided sample in order to produce a prediction.

Visiting the XML
----------------
*cPMML* defines one class for each PMML XML element. Once the XML is read, it is visited recursively and,
for each XML node encountered, the corresponding object instance is constructed.

Most of the computation takes place in this phase as **the idea is to precompute as much as possible
in order to speed up scoring**.

In general, any object part of a cpmml::Model is meant to be immutable. With the exception of the following classes:

    - *Sample*
    - *Feature*
    - *Value*

Indeed the previous may be used to store the user input for scoring. In this case,
the values are changed rather than copied, to speed-up scoring.

Feature Fields
--------------
The PMML serialization of a model contains the definition of multiple fields which serve as input for the model.
This fields can be defined in:

    DataDictionary
        It defines the fields provided as input from the user.
    TransformationDictionary
        It defines fields derived from the ones in DataDictionary
        by applying preprocessing transformations (standardization, discretization, etc.)
    LocalTransformations
        As TransformationDictionary but with local scope.
    Output
        It defines fields derived from the output of the model scoring by applying
        postprocessing transformations (probability computation, etc.)
    MiningSchema
        It just contains the reference to which fields, among the previously defined ones,
        are actually used by the model.

In *cPMML* all fields are indexed through the class *Indexer*.
This is done to allow accessing the fields through an integer index in order to improve performance. \
A shared instance of Indexer is used to share the associations integer→fieldname.

======
Core
======

.. doxygenclass:: BuiltInFunction
.. doxygenclass:: DataType
.. doxygenclass:: DataField
.. doxygenclass:: DataDictionary
.. doxygenclass:: Indexer
.. doxygenclass:: Closure
.. doxygenclass:: DagBuilder
.. doxygenclass:: DerivedField
.. doxygenclass:: FieldUsageType
.. doxygenclass:: Header
.. doxygenclass:: InternalEvaluator
.. doxygenclass:: InternalModel
.. doxygenclass:: InternalScore
.. doxygenclass:: IntervalBuilder
.. doxygenclass:: InvalidValueTreatmentMethod
.. doxygenclass:: MiningField
.. doxygenclass:: MiningFunction
.. doxygenclass:: MiningSchema
.. doxygenclass:: MissingValueTreatmentMethod
.. doxygenclass:: ModelBuilder
.. doxygenclass:: OpType
.. doxygenclass:: OutlierTreatmentMethod
.. doxygenclass:: Predicate
.. doxygenclass:: PredicateOpType
.. doxygenclass:: PredicateType
.. doxygenclass:: PredicateBuilder
.. doxygenclass:: Property
.. doxygenclass:: Sample
.. doxygenclass:: Feature
.. doxygenclass:: Target
.. doxygenclass:: TransformationDictionary
.. doxygenclass:: string_view
.. doxygenclass:: XmlNode
.. doxygenclass:: Value

==========
Expression
==========

.. doxygenclass:: Expression
.. doxygenclass:: Apply
.. doxygenclass:: Constant
.. doxygenclass:: Discretize
.. doxygenclass:: FieldRef
.. doxygenclass:: SimpleFieldRef
.. doxygenclass:: MapValues
.. doxygenclass:: NormContinuous
.. doxygenclass:: NormDiscrete
.. doxygenclass:: ExpressionType
.. doxygenclass:: ExpressionBuilder

======
Output
======

.. doxygenclass:: OutputDictionary
.. doxygenclass:: OutputField
.. doxygenclass:: OutputExpression
.. doxygenclass:: OutputExpressionType
.. doxygenclass:: OutputExpressionBuilder
.. doxygenclass:: PredictedValue
.. doxygenclass:: TransformedValue
.. doxygenclass:: Probability

=========
Ensembles
=========

.. doxygenclass:: EnsembleEvaluator
.. doxygenclass:: EnsembleModel
.. doxygenclass:: MultipleModelMethod
.. doxygenclass:: Segment

=========
TreeModel
=========

.. doxygenclass:: TreeEvaluator
.. doxygenclass:: TreeModel
.. doxygenclass:: TreeScore
.. doxygenclass:: Node
.. doxygenclass:: ScoreDistribution

===============
RegressionModel
===============

.. doxygenclass:: RegressionEvaluator
.. doxygenclass:: RegressionModel
.. doxygenclass:: RegressionTable
.. doxygenclass:: NumericPredictor
.. doxygenclass:: CategoricalPredictor
.. doxygenclass:: PredictorTerm
.. doxygenclass:: SingleNormalizationMethodBuilder
.. doxygenclass:: MultiNormalizationMethodBuilder
.. doxygenclass:: NormalizationMethodType
.. doxygenclass:: RegressionScore

====
Math
====

.. doxygengroup:: GenericMathFunctions
.. doxygengroup:: NormalizationMethods

=========
TreeTable
=========

.. doxygenclass:: TreeTable
.. doxygenclass:: TreeTableNode

=====
Utils
=====

.. doxygengroup:: Utils
.. doxygenclass:: CSVReader

