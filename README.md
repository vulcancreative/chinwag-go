[![Build Status](https://travis-ci.org/vulcancreative/chinwag-go.svg?branch=master)](https://travis-ci.org/vulcancreative/chinwag-go)

## Introduction

Chinwag, other than being a funny word, is an open-source, developer toolset used for text-synthesis. The goal is to allow for developers and designers (with a little programming experience) to be able to rapidly prototype text-heavy interfaces, but in a fun manner.

It is primarily written in C99 (for the sake of speed and portability), but has many offical language bindings, covering C, Ruby, Python, Swift, and Go.

The library universally features:

* Powerful dictionary type with accompanying routines
* Several embedded dictionaries
* Easy custom dictionary creation and use
* Easy output configuration
* C99-based for speed and portability
* Zero dependencies

## Installation


	EXAMPLE IN
	go get github.com/vulcanca/chinwag-go

## Versioning

When we make releases to the API, we strive for consistency across all of the various, language-flavors. Meaning -- when we release an update to the core Chinwag API (in C99), we update all sister components. This should guarantee a consistent version release number across all equivalent libraries.

	EXAMPLE IN
	import (
		"fmt"
		"github.com/vulcanca/chinwag-go"
	)
	fwt.Println(chinwag.Version)

	EXAMPLE OUT
	1.2.3

## Dictionaries

To generate output, you need to open a dictionary object. The dictionary can be blank, pulled from a custom token file, or loaded from one of Chinwag's embedded options -- `Seussian` or `Latin`.

### Opening an Embedded Dictionary

Typically the easiest way to [generate output](Generation) is to simply use one of the library's embedded dictionaries -- either `Seussian` or `Latin`.

These are installed programmatically, and have their own specific method for access. This is advantageous when utilizing multiple dicitonaries and caching to a global is not an option, as IO bottlenecking isn't a factor.

	EXAMPLE IN
	import "github.com/vulcanca/chinwag-go"
	seuss := chinwag.OpenEmbedded("Seussian")
	latin := chinwag.OpenEmbedded("Latin")

	EXAMPLE OUT
	seuss: {
		Name(): "Seussian",
		Length(): 1096,
		_: [
			[I, a], [TV, am, an, as, at, be, ...
			[Mordecai Ali Van Allen O'Shea]
		]
	}

	latin: {
		Name(): "Latin",
		Length(): 35664,
		_: [
			[a, b, c, d, e, f, k, l, m, n, o, ...
			semicircumferentia, supersubstantialis, supertriparticular]
		]
	}

### Opening a Custom Dictionary

Opening a custom dictionary is very similar to opening an embedded dictionary. Typically the only drawback, however, is that it is a little slower, given that there is often some I/O overhead. Custom dictionaries do need to be [checked for errors](Errors) and [sorted](SortingAndPruning), as well, prior to [generation](Generation).

If you need a valid, custom dictionary to test against, we recommend our [Noise dictionary](DownloadNoiseDictionary). It has several thousand entries, and will have no problem passing any and all internal validation procedures.

	EXAMPLE IN
	import (
		"log"
		"path"
		"io/ioutil"
		"github.com/vulcanca/chinwag-go"
	)
	filename := path.Join("dictionaries", "noise.dict")
	tokens, err := ioutil.ReadFile(filename)
	if err != nil { log.Fatal(err) }
	noise := chinwag.OpenWithNameAndTokens("Noise", tokens)

	EXAMPLE OUT
	noise: {
		Name(): "Noise",
		Length(): 18957,
		_: [
			[g, s, u, z, l, h, i, a, m, v, o, q, ...
			pzhvbzvnsdozcuxpgldrwylvedosnbbktoyi]
		]
	}

### Opening a Blank Dictionary

While having a blank dictionary is not particularly useful, you can append to it after the fact, gradually building a functional dictionary. Blank, unnamed dictionaries have no internal heap allocations, when first initialized.

	EXAMPLE IN
	import "github.com/vulcanca/chinwag-go"
	blank := chinwag.Open()

	EXAMPLE OUT
	blank: {
		Name(): "",
		Length(): 0,
		_: []
	}

### Examining Dictionaries

If there is ever a reason you need to visually debug a dictionary, each of our libraries supports a visualization component. This forces the dictionary instance to spill its guts, via the command-line.

	EXAMPLE IN
	import "github.com/vulcanca/chinwag-go"
	seuss := chinwag.OpenEmbedded("Seussian")
	chinwag.Print(seuss)

	EXAMPLE OUT
	[[I, a], [TV, am, an, as, at, be, ...
	[Dibble Dibble Dibble Dribble], [Mordecai Ali Van Allen O'Shea]]

### Dictionary Arithmetic

While generation requires a dictionary to be sorted by length, it is also best-practice to prune your dictionary of repeat elements. This is done directly by our tools [prepare_dict](Universal) and [compile_dict](ChinwagOnly), during custom .dict creation. However, blank dictionaries, which are gradually built upon, require inline cleanup prior to use.
Occasionally, one needs to make modifications directly to a dictionary instance. We allow for this via Enumerators (where applicable), or library routines, modifiying the instance's internal entries directly. This is particularly useful for, say, converting all entries to uppercase.
When using a newer, more dynamic language, such as Ruby, Python, Swift, or Go, memory management isn't too much of an issue, and all dictionary objects are automatically released for you (via reference counting or garbage collection). However, in C, or when you simply need some free memory, it is beneficial to be able to close the dictionary at hand, when no longer in use.
Upon loading a foreign dictionary, it is crucial to test its validity, prior to use. This checks that the library will be capable of understanding the dictionary format properly, and, if so, ensures adequate randomization for our synthesis algorithms. Depending on the security risks potentially present in your library of choice (the lower the level, the higher the risk), it may be a wise decision to terminate on certain circumstances.

Embedded dictionaries have already been thoroughly tested, and need no further validation. This, in turn, grants the embedded resources an additional speed boost.
With a valid dictionary in-hand, generating output is an incredibly easy task. One needs to simply specify the `output type` and `output amount(s)`, passing the dictionary reference as an argument, and the library will handle the rest. Output is always returned in terms of your library's character-array-equivalent implementation, typically a String class.

It is possible to allow for generation using only the defaults, and, subsequently, modifying the defaults, to allow for succinct operation.
Chinwag falls under the [MIT License](http://opensource.org/licenses/MIT). Copyright © 2015 Vulcan Creative, LLC.
Babble is privately licensed. Copyright © 2015 Vulcan Creative, LLC.

Unlike Babble, Chinwag, and all its sister language implementations, are completely open-source and expandable. [Contribution](https://github.com/vulcancreative) is encouraged. We aim to regularly fold in end-user contributions, and, occasionally, they creep into Babble, as well.

Babble was conceptualized in part by Interface Designer [Ben Bate](http://benbate.com), without whom none of this would be possible. Thank you, Ben!

Gorgeous screenshot backing-image provided by [Stacey Guptill](http://www.inspiredfromtime.com). Thanks, Stacey!
