package chinwag

import (
  "os"
  "fmt"
  "log"
  "bytes"
  "unsafe"
  "reflect"
  "io/ioutil"
)

/*
#include "chinwag.h"
*/
import "C"

const Version = "1.2.3"

type CWDict C.struct_dictionary_container_type

type CWType uint8
const (
  Letters CWType = CWType(C.CW_LETTERS)
  Words CWType = CWType(C.CW_WORDS)
  Sentences CWType = CWType(C.CW_SENTENCES)
  Paragraphs CWType = CWType(C.CW_PARAGRAPHS)
)

type ErrorType string
const (
  InvalidOutputType ErrorType = "CWError.InvalidOutputType"
  MinLessThanOne ErrorType = "CWError.MinLessThanOne"
  MaxLessThanMin ErrorType = "CWError.MaxLessThanMin"
  MaxTooHigh ErrorType = "CWError.MaxTooHigh"
  DictTooSmall ErrorType = "CWError.DictTooSmall"
  DictUnsortable ErrorType = "CWError.DictUnsortable"
  DictUnknown ErrorType = "CWError.DictUnknown"
)

var Delimiters = C.GoString(C.CW_DELIMITERS)

func Generate(dict CWDict, kind CWType, min, max uint64) (string, *ErrorType) {
  cwerror := dict.Validate()
  if cwerror != nil { return "", cwerror }

  var err C.cwerror_t
  result := C.chinwag(C.cw_t(kind), C.ulong(min), C.ulong(max),
  C.struct_dictionary_container_type(dict), &err)

  if result == nil {
    var go_error ErrorType
    if err == C.CWERROR_INVALID_OUTPUT_TYPE {
      go_error = InvalidOutputType
    } else if err == C.CWERROR_MIN_LESS_THAN_ONE {
      go_error = MinLessThanOne
    } else if err == C.CWERROR_MAX_LESS_THAN_MIN {
      go_error = MaxLessThanMin
    } else if err == C.CWERROR_MAX_TOO_HIGH {
      go_error = MaxTooHigh
    } else {
      go_error = DictUnknown
    }

    return "", &go_error
  }

  return C.GoString(result), nil
}

func Open() CWDict {
  return CWDict(C.cwdict_open())
}

func OpenWithName(name string) CWDict {
  dict := CWDict(C.cwdict_open())
  dict.SetName(name)

  return dict
}

func OpenEmbedded(name string) CWDict {
  dict := CWDict(C.cwdict_open())

  delimiters := C.CString(Delimiters)
  defer C.free(unsafe.Pointer(delimiters))

  switch name {
  case "Seussian", "seussian", "seuss", "Seuss":
    cname := C.CString("Seussian")
    defer C.free(unsafe.Pointer(cname))

    dict = CWDict(C.cwdict_open_with_name_and_tokens(cname,
    C.dict_seuss, delimiters))
  case "Latin", "latin":
    cname := C.CString("Latin")
    defer C.free(unsafe.Pointer(cname))

    dict = CWDict(C.cwdict_open_with_name_and_tokens(cname,
    C.dict_latin, delimiters))
  default:
    cname := C.CString(name)
    defer C.free(unsafe.Pointer(cname))
    dict.name = cname
  }

  return dict
}

func OpenWithTokens(filename string) CWDict {
  dict := CWDict(C.cwdict_open())

  delimiters := C.CString(Delimiters)
  defer C.free(unsafe.Pointer(delimiters))

  contents, err := ioutil.ReadFile(filename)
  if err != nil { log.Fatal(err) }

  ccontents := C.CString(string(contents))
  defer C.free(unsafe.Pointer(ccontents))
  dict = CWDict(C.cwdict_open_with_tokens(ccontents, delimiters))

  return dict
}

func OpenWithNameAndTokens(name, filename string) CWDict {
  dict := CWDict(C.cwdict_open())

  delimiters := C.CString(Delimiters)
  defer C.free(unsafe.Pointer(delimiters))

  contents, err := ioutil.ReadFile(filename)
  if err != nil { log.Fatal(err) }

  ccontents := C.CString(string(contents))
  defer C.free(unsafe.Pointer(ccontents))

  cname := C.CString(name)
  defer C.free(unsafe.Pointer(cname))

  dict = CWDict(C.cwdict_open_with_name_and_tokens(cname, ccontents,
  delimiters))

  return dict
}

func (dict CWDict) Name() string {
  return C.GoString(dict.name)
}

func (dict *CWDict) SetName(name string) *CWDict {
  dict.name = C.CString(name)
  return dict
}

func (dict *CWDict) AppendWord(word string) *CWDict {
  cword := C.CString(word)
  defer C.free(unsafe.Pointer(cword))
  *dict =
  CWDict(C.cwdict_place_word(C.struct_dictionary_container_type(*dict),
  cword))

  return dict
}

func (dict *CWDict) AppendWords(words ...string) *CWDict {
  for _, word := range words {
    cword := C.CString(word)
    defer C.free(unsafe.Pointer(cword))
    *dict =
    CWDict(C.cwdict_place_word(C.struct_dictionary_container_type(*dict),
    cword))
  }

  return dict
}

func (dict *CWDict) AppendSlice(words []string) *CWDict {
  for _, word := range words {
    cword := C.CString(word)
    defer C.free(unsafe.Pointer(cword))
    *dict =
    CWDict(C.cwdict_place_word(C.struct_dictionary_container_type(*dict),
    cword))
  }

  return dict
}

func (dict *CWDict) PlaceWord(word string) *CWDict {
  cword := C.CString(word)
  defer C.free(unsafe.Pointer(cword))
  *dict =
  CWDict(C.cwdict_place_word_strict(C.struct_dictionary_container_type(*dict),
  cword))

  return dict
}

func (dict *CWDict) PlaceWords(words ...string) *CWDict {
  for _, word := range words {
    cword := C.CString(word)
    defer C.free(unsafe.Pointer(cword))
    *dict =
    CWDict(C.cwdict_place_word_strict(C.struct_dictionary_container_type(*dict),
    cword))
  }

  return dict
}

func (dict *CWDict) PlaceSlice(words []string) *CWDict {
  for _, word := range words {
    cword := C.CString(word)
    defer C.free(unsafe.Pointer(cword))
    *dict =
    CWDict(C.cwdict_place_word_strict(C.struct_dictionary_container_type(*dict),
    cword))
  }

  return dict
}

func (dict *CWDict) Sort() {
  *dict = CWDict(C.cwdict_sort(C.struct_dictionary_container_type(*dict)))
}

func (dict CWDict) IsSorted() bool {
  return bool(C.struct_dictionary_container_type(dict).sorted)
}

func (dict *CWDict) Prune() {
  *dict = CWDict(C.cwdict_prune(C.struct_dictionary_container_type(*dict),
  false))
}

func (dict *CWDict) Clean() {
  *dict = CWDict(C.cwdict_clean(C.struct_dictionary_container_type(*dict)))
}

// in-place modification
func (dict *CWDict) Tweak(fn func(string)string) *CWDict {
  container := C.struct_dictionary_container_type(*dict)

  row_count := int(container.count)

  var rows_raw *C.struct_dictionary_type = container.drows
  rows_hdr := reflect.SliceHeader {
    Data: uintptr(unsafe.Pointer(rows_raw)),
    Len: int(row_count),
    Cap: int(row_count),
  }
  rows := *(*[]C.struct_dictionary_type)(unsafe.Pointer(&rows_hdr))

  for _, r := range rows {
    word_count := int(r.count)

    var words_raw **C.char = r.words
    words_hdr := reflect.SliceHeader {
      Data: uintptr(unsafe.Pointer(words_raw)),
      Len: int(word_count),
      Cap: int(word_count),
    }
    words := *(*[]*C.char)(unsafe.Pointer(&words_hdr))

    for j, w := range words {
      mod := C.GoString(w); mod = fn(mod)
      words[j] = C.CString(mod)
    }
  }

  return dict
}

func (dict CWDict) Clone() CWDict {
  return CWDict(C.cwdict_clone(C.struct_dictionary_container_type(dict)))
}

func (dict CWDict) Dup() CWDict {
  return CWDict(C.cwdict_dup(C.struct_dictionary_container_type(dict)))
}

// exclude
func (dict CWDict) Exclude(word string) bool {
  return bool(C.cwdict_exclude(C.struct_dictionary_container_type(dict),
  C.CString(word)))
}

// include
func (dict CWDict) Include(word string) bool {
  return bool(C.cwdict_include(C.struct_dictionary_container_type(dict),
  C.CString(word)))
}

// validate
func (dict CWDict) Validate() *ErrorType {
  var err C.cwerror_t
  if !C.cwdict_valid(C.struct_dictionary_container_type(dict), &err) {
    var go_error ErrorType
    if err == C.CWERROR_DICT_TOO_SMALL {
      go_error = DictTooSmall
    } else if err == C.CWERROR_DICT_UNSORTABLE {
      go_error = DictUnsortable
    } else {
      go_error = DictUnknown
    }

  return &go_error
  }

  return nil
}

// equal
func (dict CWDict) Equal(against CWDict) bool {
  return bool(C.cwdict_equal(C.struct_dictionary_container_type(dict),
  C.struct_dictionary_container_type(dict)))
}

// inequal
func (dict CWDict) Inequal(against CWDict) bool {
  return bool(C.cwdict_inequal(C.struct_dictionary_container_type(dict),
  C.struct_dictionary_container_type(dict)))
}

func (dict CWDict) Length() uint64 {
  return uint64(C.cwdict_length(C.struct_dictionary_container_type(dict)))
}

func (dict CWDict) Size() uint64 {
  return uint64(C.cwdict_size(C.struct_dictionary_container_type(dict)))
}

func (dict CWDict) Count() uint64 {
  return uint64(C.cwdict_size(C.struct_dictionary_container_type(dict)))
}

// largest
func (dict CWDict) Largest() uint32 {
  return uint32(C.cwdict_largest(C.struct_dictionary_container_type(dict)))
}

// sample
func (dict CWDict) Sample() string {
  return C.GoString(C.cwdict_sample(C.struct_dictionary_container_type(dict)))
}

// join
func (dict CWDict) Join(joiner string) string {
  var result bytes.Buffer
  container := C.struct_dictionary_container_type(dict)

  row_count := int(container.count)

  var rows_raw *C.struct_dictionary_type = container.drows
  rows_hdr := reflect.SliceHeader {
    Data: uintptr(unsafe.Pointer(rows_raw)),
    Len: int(row_count),
    Cap: int(row_count),
  }
  rows := *(*[]C.struct_dictionary_type)(unsafe.Pointer(&rows_hdr))

  for i, r := range rows {
    word_count := int(r.count)

    var words_raw **C.char = r.words
    words_hdr := reflect.SliceHeader {
      Data: uintptr(unsafe.Pointer(words_raw)),
      Len: int(word_count),
      Cap: int(word_count),
    }
    words := *(*[]*C.char)(unsafe.Pointer(&words_hdr))

    for j, w := range words {
      result.WriteString(C.GoString(w))

      if j < word_count - 1 { result.WriteString(joiner) }
    }

    if i < row_count - 1 { result.WriteString(joiner) }
  }

  return result.String()
}

// close
func (dict *CWDict) Close() *CWDict {
  *dict = CWDict(C.cwdict_close(C.struct_dictionary_container_type(*dict)))
  return dict
}

func (dict CWDict) String() string {
  var result bytes.Buffer
  container := C.struct_dictionary_container_type(dict)

  row_count := int(container.count)

  result.WriteString("[")
  var rows_raw *C.struct_dictionary_type = container.drows
  rows_hdr := reflect.SliceHeader {
    Data: uintptr(unsafe.Pointer(rows_raw)),
    Len: int(row_count),
    Cap: int(row_count),
  }
  rows := *(*[]C.struct_dictionary_type)(unsafe.Pointer(&rows_hdr))

  for i, r := range rows {
    result.WriteString("[")

    word_count := int(r.count)

    var words_raw **C.char = r.words
    words_hdr := reflect.SliceHeader {
      Data: uintptr(unsafe.Pointer(words_raw)),
      Len: int(word_count),
      Cap: int(word_count),
    }
    words := *(*[]*C.char)(unsafe.Pointer(&words_hdr))

    for j, w := range words {
      result.WriteString(C.GoString(w))

      if j < word_count - 1 { result.WriteString(", ") }
    }

    result.WriteString("]")

    if i < row_count - 1 { result.WriteString(", ") }
  }

  result.WriteString("]")
  return result.String()
}

func (dict CWDict) Print() {
  fmt.Printf("%s\n", dict.String())
}

func ErrString(dict CWDict, err *ErrorType) string {
  var result string

  switch *err {
  case InvalidOutputType:
    msg := C.GoString(C.cwerror_string(C.struct_dictionary_container_type(dict),
    C.CWERROR_INVALID_OUTPUT_TYPE))
    result = fmt.Sprintf("%s : %s", *err, msg)
  case MinLessThanOne:
    msg := C.GoString(C.cwerror_string(C.struct_dictionary_container_type(dict),
    C.CWERROR_MIN_LESS_THAN_ONE))
    result = fmt.Sprintf("%s : %s", *err, msg)
  case MaxLessThanMin:
    msg := C.GoString(C.cwerror_string(C.struct_dictionary_container_type(dict),
    C.CWERROR_MAX_LESS_THAN_MIN))
    result = fmt.Sprintf("%s : %s", *err, msg)
  case MaxTooHigh:
    msg := C.GoString(C.cwerror_string(C.struct_dictionary_container_type(dict),
    C.CWERROR_MAX_TOO_HIGH))
    result = fmt.Sprintf("%s : %s", *err, msg)
  case DictTooSmall:
    msg := C.GoString(C.cwerror_string(C.struct_dictionary_container_type(dict),
    C.CWERROR_DICT_TOO_SMALL))
    result = fmt.Sprintf("%s : %s", *err, msg)
  case DictUnsortable:
    msg := C.GoString(C.cwerror_string(C.struct_dictionary_container_type(dict),
    C.CWERROR_DICT_UNSORTABLE))
    result = fmt.Sprintf("%s : %s", *err, msg)
  default:
    msg := C.GoString(C.cwerror_string(C.struct_dictionary_container_type(dict),
    C.CWERROR_DICT_UNKNOWN))
    result = fmt.Sprintf("%s : %s", *err, msg)
  }

  return result
}

func Warn(dict CWDict, err *ErrorType) {
  fmt.Println(ErrString(dict, err))
}

func Fatal(dict CWDict, err *ErrorType) {
  Warn(dict, err)
  os.Exit(1)
}
