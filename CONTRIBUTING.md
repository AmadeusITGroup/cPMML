# Contributing

When contributing to this repository, please first discuss the change you wish to make via issue,
email, or any other method with the owners of this repository before making a change.

## Pull Requests

### Licensing of contributed material

Keep in mind as you contribute, that code, docs and other material submitted to the 
*cPMML* project are considered licensed under the terms of the [MIT license](LICENSE.md).

### Version control branching

* Always **make a new branch** for your work, no matter how small.
* **Don't submit unrelated changes in the same branch/pull request**
* **Base your new branch off of the appropriate branch** on the main
  repository:
    * **Bug fixes** should be based on the branch named after the **oldest
      supported release line** the bug affects.
    * **New features** should branch off of **the 'master' branch**.

### Code formatting

* **Follow the style you see used in the primary repository**
* For indentation, *cPMML* follows a slightly modified version of 
  [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
  **The utility script [format.sh](format.sh) has been included to help you enforce the requested style**.
  
### Code quality

In order to ensure contributions do not impact the code quality of the primary repository, changes must not introduce
any new error for:
* **Static analysis**. The checks can be performed through the **utility script** [run_cppcheck.sh](runcppcheck.sh).
* **Memory errors detection**. The checks can be performed through the **utility script** [run_valgrind.sh](runvalgrind.sh).

### Multithreading

*cPMML* provides parallel execution for scoring of ensemble models (eg. Random Forest Model, Gradient Boosted Trees, etc.).

Even though multithreaded parallelization is included by default, it must always be considered as an option for the scoring.
In other words, when contributing to *cPMML*, sequential execution must be provided before implementing an optimized version using
multithreaded parallelization.
  
### Documentation

Patches without documentation will be returned to sender.  By
"documentation" we mean  with:

* **Text file** with technical details about the changes made (input parameters, exceptions, etc.).
* **Text file** with examples of use in case of new features.
* **Doxygen docstrings** with brief description and link to the corresponding PMML element documentation (if present).

Maintainers will then merge your docstrings with the web version of the documentation.

### Tests

Every change should be submitted along with the corresponding tests. 

In particular:
* Every contributor is asked to write **unit tests** for his code. In particular, the contributor is asked to:
    * Write a new test, verifying it fails.
    * Add his contribution.
    * Check the test now passes.

* In case of new features introducing support for additional PMML elements, the contributor is asked to 
  provide:
    * A **PMML model** containing the given element.
    * A **CSV file** containing the samples and the corresponding ground-truth predictions.
      
* New contributions should not prevent the success of existing tests. 
    A collection of tests is automatically performed on every build to help you quickly check the impact
     of your contributions.
     
### Benchmarks

*cPMML* focuses on high performance, both in terms of memory and scoring time.

For this reason maintainers may reject your contribution in case it has too big impact on performance.

A collection of benchmarks is automatically performed at each build to help you quickly check the 
impact of your contributions.



## Issues

### Due diligence

Before submitting an issue, please do the following:

* Perform **basic troubleshooting** steps:
    * **Make sure you're on the latest version.** If you're not on the most
      recent version, your problem may have been solved already! Upgrading is
      always the best first step.
    * **Try older versions.** If you're already *on* the latest release, try
      rolling back a few minor versions (e.g. if on 1.7, try 1.5 or 1.6) and
      see if the problem goes away. This will help the devs narrow down when
      the problem first arose in the commit log.
    * **Try switching up dependency versions.** If the software in question has
      dependencies (other libraries, etc) try upgrading/downgrading those as
      well.

* **Search the project's issue tracker** to make sure it's not a known
  issue.
* If you don't find a pre-existing issue, consider **checking with *cPMML* repo admins**
 in case the problem is non-issue-related.

### What to put in your issue report
Make sure your report gets the attention it deserves: issue reports with missing
information may be ignored or punted back to you, delaying a fix.  The below
constitutes a bare minimum; more info is almost always better:

* **What version of the compiler are you
  using?** (g++? clang? MinGW?)
* **What operating system are you on?** Windows? (Vista? 7? 32-bit? 64-bit?)
  Mac OS X?  (10.7.4? 10.9.0?) Linux? (Which distro? Which version of that
  distro? 32 or 64 bits?) Again, more detail is better.
* **Which version or versions of the software are you using?** Ideally, you
  followed the advice above and have ruled out (or verified that the problem
  exists in) a few different versions.
* **How can the developers recreate the issue on their end?** If possible,
  include a copy of your code, the command you used to invoke it, and the full
  output of your run (if applicable.)

    * A common tactic is to pare down your code until a simple (but still
      issue-causing) "base case" remains. Not only can this help you identify
      problems which aren't real issues, but it means the developer can get to
      fixing the issue faster.