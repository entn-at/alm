# [Change Log](https://github.com/anyks/alm/archive/release.tar.gz)

## [3.0.0](https://github.com/anyks/alm/archive/v3.0.0.tar.gz) New version

## [2.1.6](https://github.com/anyks/alm/archive/v2.1.6.tar.gz) Other optimization

## [2.1.5](https://github.com/anyks/alm/archive/v2.1.5.tar.gz) Bug fixes and python version
- Python version ALM [pip](https://pypi.org/project/anyks-lm)

## [2.1.4](https://github.com/anyks/alm/archive/v2.1.4.tar.gz) Bug fixes and new method
### Method
- Bridge to Python3 by [swig](https://github.com/swig/swig)
### Params
- flag allowing the use of a hyphen as part of the word

## [2.1.3](https://github.com/anyks/alm/archive/v2.1.3.tar.gz) Other optimization

## [2.1.2](https://github.com/anyks/alm/archive/v2.1.2.tar.gz) Other optimization

## [2.1.1](https://github.com/anyks/alm/archive/v2.1.1.tar.gz) Other optimization

## [2.1.0](https://github.com/anyks/alm/archive/v2.1.0.tar.gz) Bug fixes

## [2.0.9](https://github.com/anyks/alm/archive/v2.0.9.tar.gz) Bug fixes and new params
- type of mixing language models
- context length for Bayes mixture LM
- log likelihood scale for bayes mixing

## [2.0.8](https://github.com/anyks/alm/archive/v2.0.8.tar.gz) Bug fixes

## [2.0.7](https://github.com/anyks/alm/archive/v2.0.7.tar.gz) Bug fixes and new method
### Method
- **mix** - mixing language models method
### Params
- file address *.arpa for mixing
- weight lambda first model for mixing
- flag to mixing language models in the backward direction

## [2.0.6](https://github.com/anyks/alm/archive/v2.0.6.tar.gz) Bug fixes

## [2.0.5](https://github.com/anyks/alm/archive/v2.0.5.tar.gz) Bug fixes

## [2.0.4](https://github.com/anyks/alm/archive/v2.0.4.tar.gz) Bug fixes and new methods
### Methods
- **find** - ngrams search method in text
- **sentences** - sentences generation method
### Params
- flag to accurate check ngram
- file address *.txt for export oov words
- sentences count for generation

## [2.0.3](https://github.com/anyks/alm/archive/v2.0.3.tar.gz) Bug fixes

## [2.0.2](https://github.com/anyks/alm/archive/v2.0.2.tar.gz) Bug fixes

## [2.0.1](https://github.com/anyks/alm/archive/v2.0.1.tar.gz) Optimization and bug fixes

## 2.0.0 New methods and bug fixes
### Methods
- **ppl** - perplexity calculation method
- **checktext** - text validation method
- **fixcase** - words case correction method
- **counts** - method counts of ngrams in the text
### Params
- ngram size for the counts method
- text to be processed as a string
- file address *.txt for texts export
- file address *.txt or dir path for texts import
- flag arpa file loading without pre-processing the words

## 1.0.4 Performance optimization and bug fixes
- flag to save intermediate data during training

## 1.0.3 Multiprocessor training and bug fixes
- flag to performing data segmentation during training
- flag to clearing temporary data during training
- segment size for training data segmentation
- number of threads for data collection
- file address *.txt or dir path for words import

## 1.0.2 Bug fixes and new method
### Method
- **vprune** - vocabulary pruning method
### Params
- flag allowing the use of words consisting of mixed dictionaries
- file address for restore mixed words for import

## 1.0.1 Performance improvements, bug fixes
- flag to reset the frequency of an unknown word
- flag export in binary dictionary of all data
- flag to disable all tokens
- flag identifying all tokens as 〈unk〉
- list of tokens to disable
- list of tokens for identification as 〈unk〉
- file address domain zones for import
- file address abbreviations for import
