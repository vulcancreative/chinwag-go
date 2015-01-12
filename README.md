## Introduction

Chinwag, the back-end, developer toolset behind Babble, has a very accessible API. While it is implemented purely in [C99](http://en.wikipedia.org/wiki/C99), its API was crafted with a very dynamic mindset, reminiscent of its sister, scripting-language implementations (also catalogued here). We have made a diligent effort to ensure Chinwag, and, subsequently, Babble, have no external dependencies to keep track of. As a direct result, the entire Chinwag family is intended to compile with ease -- whether one is using C, Ruby, Python, Swift, or Go.

Do note -- our style of API documentation may be a bit unfamiliar to some, in that we document the implementation, along with the bare minimum needed for execution (importation statements, et cetera). We find this technique fares best with seasoned programmers, who understand the mechanics of their language of choice. It is recommend for those with less experience to study up on their language of choice, and view the following as a cursory overview.

## Installation

Installation of the API is a relatively simple procedure. Where possible, we try our best to make the appropriate library available via your language-specific package manager. This technique often fully automates the installation process, either globally or for your project (depending on your settings). Thus, if you're a more advanced user looking to install manually, cloning your language-specific Github repo would be the best approach, for integration.

Alternatively, if you own a Babble license, purchased from the Mac App Store, you can simply follow the instructions noted in the `Preference Panel`. The steps will guide you in downloading the Developer Integration package, opening, and installing. Upon installation you will have Chinwag, and all of its sister libraries installed, along with a few local utilities.

All installation is done via your machine's command-line.

	EXAMPLE IN
	go get github.com/vulcanca/chinwag-go

## Versioning

When we make releases to the API, we strive for consistency across all of the various language-specific flavors. Meaning -- when we release an update to the core Chinwag API (in C99), we update all sister components. This should guarantee a consistent version release number across all equivalent libraries.

	EXAMPLE IN
	import "github.com/vulcanca/chinwag-go"
	fwt.Println(chinwag.Version)

	EXAMPLE OUT
	1.2.3

## Dictionaries

To generate output, you need to open a new dictionary object. The dictionary can be blank, pulled from a custom token file, or loaded from one of Chinwag's embedded options -- `Seussian` or `Latin`. Depending on your library implementation, the dictionary object may be a struct or native class-type.

### Opening an Embedded Dictionary

Typically the easiest way to [generate output](Generation) is to simply use one of the library's embedded dictionaries -- either `Seussian` or `Latin`.

Where applicable, a standardized delimiter character-array/String (such as `CW_DELIMITERS`) is passed to the opening method. This is merely a list of characters, and is used to determine what barriers a dictionary entry from another. By default, this delimiter list accounts for line-breaks (`\r` and `\n`), commas (`,`), semicolons (`;`), colons (`:`), and junk characters (`\034`).

	EXAMPLE IN
	import "github.com/vulcanca/chinwag-go"
	seuss := chinwag.Open("Seussian")
	latin := chinwag.Open("Latin")

	EXAMPLE OUT

### Opening a Custom Dictionary

Opening a custom dictionary is very similar to opening an embedded dictionary. Typically the only drawback, however, is that it is a little slower, given that there is often some I/O overhead. Custom dictionaries do need to be [checked for errors](Errors), as well, prior to [generation](Generation).

If you need a valid, custom dictionary to test against, we recommend our [Noise dictionary](DownloadNoiseDictionary). It has several thousand entries, and will have no problem passing any and all internal validation procedures. Otherwise, if you would like to create our own, we'd be happy to [show you how](CreatingDictionaries).


### Opening a Blank Dictionary

While having a blank dictionary is not particularly useful, you can append to it after the fact, gradually building a functional dictionary. Blank, unnamed dictionaries have no internal allocations, and, therefore, consume no memory on the heap.

	EXAMPLE IN
	import "github.com/vulcanca/chinwag-go"
	blank := chinwag.Open()

	EXAMPLE OUT

### Examining Dictionaries

If there is ever a reason you need to visually debug a dictionary, each of our libraries supports a visualization component. This forces the dictionary instance to spill its guts, via your command-line, debug interface.

	EXAMPLE IN
	import "github.com/vulcanca/chinwag-go"
	seuss := chinwag.Open("Seussian")
	chinwag.Print(seuss)

	EXAMPLE OUT

### Dictionary Arithmetic

Whether using an embedded dictionary, or something custom, you can concatenate new entries in the form of character-arrays or Strings (depending on your library implementation). This is particularly useful if you have a blank dictionary, and gradually want to build upon it by adding in information dynamically.

	EXAMPLE IN
	import "github.com/vulcanca/chinwag-go"
	blank := chinwag.Open()

	EXAMPLE OUT

### Sorting and Pruning

While generation requires a dictionary to be sorted by length, it is also best-practice to prune your dictionary of repeat elements. This is done directly by our tools [prepare_dict](Universal) and [compile_dict](ChinwagOnly), during custom .dict creation. However, blank dictionaries, which are gradually built upon, require inline cleanup prior to use.


### Duplication

As dictionaries are rooted as complex structs in C99, and require a variety of resources to initalize and close, duplication is a slightly complex procedure.

Nevertheless, we allow deep copies, via our range of library implementations. This is done by copying all pointers (at a very low-level) into a brand new dictionary, as to not cause any cross-referential issues or leaks. Duplication will respect any sorting or pruning that has been done previously to the dictionary being copied, and will have a new address in memory.

	EXAMPLE IN
	import "github.com/vulcanca/chinwag-go"
	seuss := chinwag.Open("Seussian")
	copy := chinwag.Clone(seuss)

	EXAMPLE OUT

### In-Place Modification

Occasionally, one needs to make modifications directly to a dictionary instance. We allow this via Enumerators (where applicable), or library routines, modifiying the instance's internal entries directly. This is particularly useful for, say, converting all entries to uppercase.


### Closing a Dictionary

When using a newer, more dynamic language, such as Ruby, Python, Swift, or Go, memory management isn't too much of an issue, and all dictionary objects are automatically released for you (via reference counting or garbage collection). However, in C, or when you simply need some free memory, it is beneficial to be able to close the dictionary at hand, when no longer in use.

	EXAMPLE IN
	import "github.com/vulcanca/chinwag-go"
	blank := chinwag.Close(seuss)
	// Clears all of seuss' internal, dynamic memory,
	// and resets it to a blank dictionary, which
	// you are free to capture

	EXAMPLE OUT

## Validation and Errors

Upon loading a foreign dictionary, it is crucial to test its validity, prior to use. This checks that the library will be capable of understanding the dictionary format properly, and, if so, ensures adequate randomization for our synthesis algorithms. Depending on the security risks potentially present in your library of choice (the lower the level, the higher the risk), it may be a wise decision to terminate on certain circumstances.

Embedded dictionaries have already been thoroughly tested, and need no further validation. This, in turn, grants the embedded resources an additional speed boost.

	EXAMPLE IN
	import "github.com/vulcanca/chinwag-go"
	blank := chinwag.Open("Seussian")
	error := chinwag.Validate(blank)

	if error != nil {
		cwdictError := error.(Error)

		switch cwdictError.Type {
			case cwdictError.TooSmall:
			case cwdictError.Unsortable:
		}
	}

	EXAMPLE OUT
	CWDictError: too few acceptable entries (0 of 300)

## Generation

With a reference to a valid dictionary, generating output is an incredibly easy task. One needs to simply specify the `output type` and `output amount(s)`, passing the dictionary reference as an argument, and the library will handle the rest. Output is always returned in terms of your library's character-array-equivalent implementation, typically a String class.

	EXAMPLE IN
	import "github.com/vulcanca/chinwag-go"
	seuss := chinwag.Open("Seussian")
	output := chinwag.Generate(chinwag.Words, 10, 20, seuss)
	// Generates ten to twenty words in Seussian

	EXAMPLE OUT

## Legal

Chinwag falls under the [MIT License](http://opensource.org/licenses/MIT). Copyright © 2015 Vulcan Creative, LLC.
Babble is privately licensed. Copyright © 2015 Vulcan Creative, LLC.

Unlike Babble, Chinwag, and all its sister language implementations, are completely open-source and expandable. [Contribution](https://github.com/vulcancreative) is encouraged. We aim to regularly fold in end-user contributions, and, occasionally, they creep into Babble, as well.

Babble was conceptualized in part by Interface Designer [Ben Bate](http://benbate.com), without whom none of this would be possible. Thank you, Ben!

Gorgeous screenshot backing-image provided by [Stacey Guptill](http://www.inspiredfromtime.com). Thanks, Stacey!
