[![Build Status](https://travis-ci.org/vulcancreative/chinwag-go.svg?branch=master)](https://travis-ci.org/vulcancreative/chinwag-go)

## Introduction


Chinwag, other than being a funny word, is an open-source, developer toolset used for text-synthesis. The goal is to allow for developers and designers (with a little programming experience) to be able to rapidly prototype text-heavy interfaces, but in a fun manner.

It is primarily written in C99 (for the sake of speed and portability), but has many official language bindings, covering C, Ruby, Python, Swift, and Go.

The library universally features:

* Powerful dictionary type with accompanying routines
* Several embedded dictionaries
* Easy custom dictionary creation and use
* Easy output configuration
* C99-based for speed and portability
* Zero dependencies

## Installation


```shell
$ go get github.com/vulcancreative/chinwag-go
```

## Versioning


When we make releases to the API, we strive for consistency across all of the various, language-flavors. Meaning &ndash; when we release an update to the core Chinwag API (in C99), we update all sister components. This should guarantee a consistent version release number across all equivalent libraries.


```go
// EXAMPLE IN
import (
	"fmt"
	"github.com/vulcancreative/chinwag-go"
)
fwt.Println(chinwag.Version)
```

```sample
// EXAMPLE OUT
1.2.3
```

## Dictionaries


To generate output, you need to open a dictionary object. The dictionary can be blank, pulled from a custom token file, or loaded from one of Chinwag's embedded options &ndash; `Seussian` or `Latin`.


### Opening an Embedded Dictionary

Typically the easiest way to [generate output](#generation) is to simply use one of the library's embedded dictionaries &ndash; either `Seussian` or `Latin`.

These are installed programmatically, and have their own specific method for access. This is advantageous when utilizing multiple dicitonaries and caching to a global is not an option, as IO bottlenecking isn't a factor.

```go
// EXAMPLE IN
import "github.com/vulcancreative/chinwag-go"
seuss := chinwag.OpenEmbedded("Seussian")
latin := chinwag.OpenEmbedded("Latin")
```

```sample
// EXAMPLE OUT
seuss: {
	Name(): "Seussian",
	Length(): 1096,
	// the following fields are implicit
	_: [
		[I, a], [TV, am, an, as, at, be, ...
		[Mordecai Ali Van Allen O'Shea]
	]
	// contains filtered or unexposed fields
}

latin: {
	Name(): "Latin",
	Length(): 35664,
	// the following fields are implicit
	_: [
		[a, b, c, d, e, f, k, l, m, n, o, ...
		semicircumferentia, supersubstantialis, supertriparticular]
	]
	// contains filtered or unexposed fields
}
```


### Opening a Custom Dictionary

Opening a custom dictionary is very similar to opening an embedded dictionary. Typically the only drawback, however, is that it is a little slower, given that there is often some I/O overhead. Custom dictionaries do need to be [checked for errors](#validation-and-errors) and [sorted](#sorting-and-pruning), as well, prior to [generation](#generation).

If you need a valid, custom dictionary to test against, we recommend our <a href="http://vulcanca.com/babble/docs/noise.dict" target="_blank">Noise dictionary</a>. It has several thousand entries, and will have no problem passing any and all internal validation procedures.

```go
// EXAMPLE IN
import (
	"log"
	"path"
	"io/ioutil"
	"github.com/vulcancreative/chinwag-go"
)
filename := path.Join("dictionaries", "noise.dict")
tokens, err := ioutil.ReadFile(filename)
if err != nil { log.Fatal(err) }
noise := chinwag.OpenWithNameAndTokens("Noise", tokens)
```

```sample
// EXAMPLE OUT
noise: {
	Name(): "Noise",
	Length(): 18957,
	IsSorted(): true,
	// the following fields are implicit
	_: [
		[g, s, u, z, l, h, i, a, m, v, o, q, ...
		pzhvbzvnsdozcuxpgldrwylvedosnbbktoyi]
	]
	// contains filtered or unexposed fields
}
```


> Note : loading a custom dictionary does invoke quite a bit of IO overhead. It is best practice to load a dictionary and cache it for the entirety of its use cycle (often in a global variable).


### Opening a Blank Dictionary

While having a blank dictionary is not particularly useful, you can append to it after the fact, gradually building a functional dictionary. Blank, unnamed dictionaries have no internal heap allocations, when first initialized.

```go
// EXAMPLE IN
import "github.com/vulcancreative/chinwag-go"
blank := chinwag.Open()
```

```sample
// EXAMPLE OUT
blank: {
	Name(): "",
	Length(): 0,
	IsSorted(): false,
	// the following fields are implicit
	_: []
	// contains filtered or unexposed fields
}
```


### Examining Dictionaries

If there is ever a reason you need to visually debug a dictionary, each of our libraries supports a visualization component. This forces the dictionary instance to spill its guts, via the command-line.

```go
// EXAMPLE IN
import "github.com/vulcancreative/chinwag-go"
seuss := chinwag.OpenEmbedded("Seussian")
chinwag.Print(seuss)
```

```sample
// EXAMPLE OUT
[[I, a], [TV, am, an, as, at, be, ...
[Dibble Dibble Dibble Dribble], [Mordecai Ali Van Allen O'Shea]]
```


### Dictionary Arithmetic

Whether using an embedded dictionary, or something custom, you can concatenate new entries in the form of strings. This is particularly useful if you have a blank dictionary, and gradually want to build upon it by adding in information dynamically.

```go
// EXAMPLE IN
import "github.com/vulcancreative/chinwag-go"
ungrouped := chinwag.Open()
grouped := chinwag.Open()
ungrouped.AddWords("these", "are", "some", "test", "words")
grouped.PlaceWords("these", "words", "will", "be", "sorted")
```

```sample
// EXAMPLE OUT
ungrouped: {
	Name(): "",
	Length(): 5,
	IsSorted(): false,
	// the following fields are implicit
	_: [
		[these, are, some, test, words]
	]
	// contains filtered or unexposed fields
}

grouped: {
	Name(): "",
	Length(): 5,
	IsSorted(): false,
	// the following fields are implicit
	_: [
		[these, words], [will], [be], [sorted]
	]
	// contains filtered or unexposed fields
}
```


### Sorting and Pruning

While generation requires a dictionary to be sorted by length, it is also best-practice to prune your dictionary of repeat elements. Cleaning both sorts and prunes.

```go
// EXAMPLE IN
import "github.com/vulcancreative/chinwag-go"
sorted := chinwag.OpenWithName("Sorted")
pruned := chinwag.OpenWithName("Pruned")
cleaned := chinwag.OpenWithName("Cleaned")
sorted.AppendWords("this", "is", "a", "quick", "test")
pruned.AppendWords("something", "something", "another", "done")
cleand.AppendWords("first", "second", "first", "second", "third")
sorted.Sort()
// orders by entry length,
// meeting generation requirements
pruned.Prune()
// removes duplicates, retains placement
// needs to be sorted before generating
cleaned.Clean()
// removes duplicates and sorts,
// meeting generation requirements
```

```sample
// EXAMPLE OUT
sorted: {
	Name(): "Sorted",
	Length(): 5,
	IsSorted(): true,
	// the following fields are implicit
	_ : [
		[a], [is], [test, this], [quick]
	]
	// contains filtered or unexposed fields
}

pruned: {
	Name(): "Pruned",
	Length(): 3,
	IsSorted(): false,
	// the following fields are implicit
	_ : [
		[something], [another], [done]
	]
	// contains filtered or unexposed fields
}

cleaned: {
	Name(): "Cleaned",
	Length(): 3,
	IsSorted(): true,
	// the following fields are implicit
	_: [
		[first, third], [second]
	]
	// contains filtered or unexposed fields
}
```


### Duplication

As dictionaries are rooted as complex structs in C99, and require a variety of resources to initialize and close, duplication is a slightly complex procedure.

Nevertheless, we allow deep copies, via the library. Duplication will respect any sorting or pruning that has been done previously to the dictionary being copied, and will have a new address in memory.

```go
// EXAMPLE IN
import "github.com/vulcancreative/chinwag-go"
seuss := chinwag.Open("Seussian")
copy := chinwag.Clone(seuss)
seuss.Close()
```

```sample
// EXAMPLE OUT
seuss: {
	Name(): "",
	Length(): 0,
	IsSorted(): false,
	// the following fields are implicit
	_ : []
	// contains filtered or unexposed fields
}

copy: {
	Name(): "Seussian",
	Length(): 1096,
	IsSorted(): true,
	// the following fields are implicit
	_: [
		[I, a], [TV, am, an, as, at, be, ...
		[Mordecai Ali Van Allen O'Shea]
	]
	// contains filtered or unexposed fields
}
```


### In-Place Modification

Occasionally, one needs to make modifications directly to a dictionary instance. We allow for modifying the instance's internal entries directly via the Tweak method, which takes a handler (function pointer). This is particularly useful for, say, converting all entries to uppercase.

```go
// EXAMPLE IN
import (
	"strings"
	"github.com/vulcancreative/chinwag-go"
)
caps := chinwag.OpenWithName("Caps")
caps.PlaceSlice([]string{"these", "words", "will", "be", "capitalized"})
caps.Tweak(strings.ToUpper)
// chinwag.Tweak requires a method
// signature of (string)string
```

```sample
// EXAMPLE OUT
caps: {
	Name(): "Caps",
	Length(): 5,
	IsSorted(): false,
	// the following fields are implicit
	_: [
		[THESE, WORDS], [WILL], [BE], [CAPITALIZED]
	]
	// contains filtered or unexposed fields
}
```


### Closing a Dictionary

By default, when closing a dictionary, a blank dictionary is returned. This value can be ignored, if desired.

```go
// EXAMPLE IN
import "github.com/vulcancreative/chinwag-go"
seuss := Chinwag.OpenEmbedded("Seussian")
latin := Chinwag.OpenEmbedded("Latin")
latin.Close()
blank := seuss.Close()
// Clears all of seuss' internal, dynamic memory,
// and resets it to a blank dictionary, which
// you are free to capture
```

```sample
// EXAMPLE OUT
seuss: {
	Name(): "",
	Length(): 0,
	// the following fields are implicit
	_: []
	// contains filtered or unexposed fields
}

latin: {
	Name(): "",
	Length(): 0,
	// the following fields are implicit
	_: []
	// contains filtered or unexposed fields
}

blank: {
	Name(): "",
	Length(): 0,
	// the following fields are implicit
	_: []
	// contains filtered or unexposed fields
}
```

## Validation and Errors


Upon loading a foreign dictionary, it is crucial to test its validity, prior to use. This checks that the library will be capable of understanding the dictionary format properly, and, if so, ensures adequate randomization for our synthesis algorithms.

Embedded dictionaries have already been thoroughly tested, and need no further validation. This, in turn, grants the embedded resources an additional speed boost.

```go
// EXAMPLE IN
import "github.com/vulcancreative/chinwag-go"
blank := chinwag.Open()
err := blank.Validate()
if err != nil {
	switch err {
	case chinwag.DictTooSmall:
		chinwag.Warn(blank, err)
	case chinwag.DictUnsortable:
		chinwag.Warn(blank, err)
	case chinwag.DictUnknown:
		chinwag.Fatal(blank, err)
	}
}
```

```sample
// EXAMPLE OUT
CWError.DictTooSmall: dict has too few acceptable entries (0 of 300)
```

## Generation


With a valid dictionary in-hand, generating output is an incredibly easy task. One needs to simply specify the `output type` and `output amount(s)`, passing the dictionary reference as an argument, and the library will handle the rest.

It is possible to allow for generation using only the defaults, and, subsequently, modifying the defaults, to allow for succinct operation.

```go
// EXAMPLE IN
import (
	"fmt"
	"github.com/vulcancreative/chinwag-go"
)
seuss := chinwag.OpenEmbedded("Seussian")
output, err := chinwag.Generate(seuss, chinwag.Words, 10, 20)
if err == nil { fmt.Println(output) }
// Prints ten to twenty words in Seussian
chinwag.defaultType = chinwag.Letters
chinwag.defaultMaxOutput = 30
// Max must be set first if higher than min
chinwag.defaultMinOutput = 30
output, err = chinwag.Gen()
if err == nil { fmt.Println(output) }
// Prints thirty letters using defaults
```

```sample
// EXAMPLE OUT
A With Monkeys Everywhere I Comes Stew Mostly Lasso Shout
Confused Congratulations When Blackbottomed

Wonderfully Her Amounts Feetae
```

## Legal


Chinwag is available under the <a href="http://opensource.org/licenses/MIT" target="_blank">MIT License</a>.<br>
Use, abuse, and please don't bite the hand that feeds you.
