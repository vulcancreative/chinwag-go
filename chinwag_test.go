package chinwag

import (
  "log"
  "path"
  "regexp"
  "strings"
  "testing"
  "unicode"
  "io/ioutil"
  "math/rand"
  "unicode/utf8"
)

const version = "1.2.3"

var latin = OpenEmbedded("Latin")

func TestChinwagVersion(t *testing.T) {
  got := Version
  if got != version  {
    t.Errorf("expected %s, got %s", version, got)
  }
}

func TestChinwagGenerateLetters(t *testing.T) {
  amount := uint64(rand.Intn(1000) + 1500)
  result, _ := Generate(latin, Letters, amount, amount)
  actual := uint64(utf8.RuneCountInString(result))

  if amount != actual {
    t.Errorf("expected %d Letters, got %d", amount, actual)
  }
}

func TestChinwagGenerateWords(t *testing.T) {
  amount := uint64(rand.Intn(1000)) + 1500
  result, _ := Generate(latin, Words, amount, amount)
  words := strings.Fields(result)
  actual := uint64(len(words))

  if amount != actual {
    t.Errorf("expected %d Words, got %d", amount, actual)
  }
}

func TestChinwagGenerateSentences(t *testing.T) {
  amount := uint64(rand.Intn(1000)) + 1500
  result, _ := Generate(latin, Sentences, amount, amount)
  sentences := regexp.MustCompile(".!?").Split(result, int(amount))
  actual := uint64(len(sentences))

  if amount != actual {
    t.Errorf("expected %d Sentences, got %d", amount, actual)
  }
}

func TestChinwagGenerateParagraphs(t *testing.T) {
  amount := uint64(rand.Intn(1000)) + 1500
  result, _ := Generate(latin, Paragraphs, amount, amount)
  paragraphs := regexp.MustCompile("\n\n").Split(result, int(amount))
  actual := uint64(len(paragraphs))

  if amount != actual {
    t.Errorf("expected %d Paragraphs, got %d", amount, actual)
  }
}

func TestChinwagGen(t *testing.T) {
  var amount uint64 = 30
  defaultType = Letters
	defaultMinOutput = amount
	defaultMaxOutput = amount
	result, _ := Gen()

  actual := uint64(utf8.RuneCountInString(result))

  if amount != actual {
    t.Errorf("expected %d Letters, got %d", amount, actual)
  }
}

func TestChinwagOpenEmbedded(t *testing.T) {
  seuss := OpenEmbedded("Seussian")
  latin := OpenEmbedded("Latin")

  if seuss.Length() <= 0 {
    t.Errorf("expected \"seuss\" to have a positive length, got (%d)",
    seuss.Length())
  }

  if seuss.String() == "[]" {
    t.Error("expected \"seuss\" to have visual representation")
  }

  if latin.Length() <= 0 {
    t.Errorf("expected \"latin\" to have a positive length, got(%d)",
    latin.Length())
  }

  if latin.String() == "[]" {
    t.Error("expected \"latin\" to have visual representation")
  }
}

func TestChinwagSetName(t *testing.T) {
  blank := Open()
  named := OpenWithName("dicklips")
  seuss := OpenEmbedded("Seussian")

  if blank.Name() != "" {
    t.Error("expected \"blank\" to have no name")
  }

  blank.SetName("blank")

  if blank.Name() != "blank" {
    t.Error("expected \"blank\" to have a name")
  }

  if named.Name() != "dicklips" {
    t.Error("expected \"named\" to be properly named")
  }

  named.SetName("whatever")

  if named.Name() != "whatever" {
    t.Error("expected \"named\" to be properly named")
  }

  if seuss.Name() != "Seussian" {
    t.Error("expected \"seuss\" to be properly named")
  }

  seuss.SetName("Geisel")

  if seuss.Name() != "Geisel" {
    t.Error("expected \"seuss\" to be properly named")
  }
}

func TestChinwagLength(t *testing.T) {
  blank := Open()
  seuss := OpenEmbedded("Seussian")

  if blank.Length() != 0 {
    t.Error("expected \"blank\" to be empty")
  }

  if blank.Size() != 0 {
    t.Error("expected \"blank\" to be empty")
  }

  if blank.Count() != 0 {
    t.Error("expected \"blank\" to be empty")
  }

  if seuss.Length() != 1096 {
    t.Errorf("expected %d, got %d", 1096, seuss.Length())
  }

  if seuss.Size() != 1096 {
    t.Errorf("expected %d, got %d", 1096, seuss.Size())
  }

  if seuss.Count() != 1096 {
    t.Errorf("expected %d, got %d", 1096, seuss.Count())
  }
}

func TestChinwagTweak(t *testing.T) {
  upper := OpenEmbedded("Seussian")
  upper.Tweak(strings.ToUpper)
  entries := regexp.MustCompile(",").Split(upper.Join(","), 1096)
  for _, e := range entries {
    for i := 0; i < len(e); i++ {
      if !unicode.IsUpper(rune(e[i])) && !unicode.IsSpace(rune(e[i])) &&
      !unicode.IsPunct(rune(e[i])) {
        t.Errorf("expected string (%s) to be upper-case", e)
      }
    }
  }
}

func TestChinwagJoin(t *testing.T) {
  seuss := OpenEmbedded("Seussian")

  file_name_1 := "chinwag_testcase_join_default"
  file_name_2 := "chinwag_testcase_join_comma"
  file_name_3 := "chinwag_testcase_join_hyphens"
  file_name_4 := "chinwag_testcase_join_dicks"

  file_1, err_1 := ioutil.ReadFile(path.Join("testcases", file_name_1))
  if err_1 != nil { log.Fatal(err_1) }
  file_2, err_2 := ioutil.ReadFile(path.Join("testcases", file_name_2))
  if err_2 != nil { log.Fatal(err_2) }
  file_3, err_3 := ioutil.ReadFile(path.Join("testcases", file_name_3))
  if err_3 != nil { log.Fatal(err_3) }
  file_4, err_4 := ioutil.ReadFile(path.Join("testcases", file_name_4))
  if err_4 != nil { log.Fatal(err_4) }

  if seuss.Join(" ") != string(file_1) {
    t.Error("unable to join \"seuss\" via \" \"")
  }

  if seuss.Join(",") != string(file_2) {
    t.Error("unable to join \"seuss\" via \",\"")
  }

  if seuss.Join(" -- ") != string(file_3) {
    t.Error("unable to join \"seuss\" via \" -- \"")
  }

  if seuss.Join(" 8===D ") != string(file_4) {
    t.Error("unable to join \"seuss\" via \" 8===D \"")
  }
}

func TestChinwagSort(t *testing.T) {
  small_mess := Open()
  small_mess.AppendWords("this", "is", "a", "quick", "test", "of", "sorting")

  if small_mess.Join(" ") != "this is a quick test of sorting" {
    t.Error("unable to join \"small_mess\" via \" \"")
  }

  if small_mess.IsSorted() {
    t.Error("\"small_mess\" is (for some reason) sorted")
  }

  small_mess.Sort()

  if small_mess.Join(" ") != "a is of this test quick sorting" {
    t.Error("unable to sort \"small_mess\"")
  }

  if !small_mess.IsSorted() {
    t.Error("\"small_mess\" unable to disclose its sorted state")
  }
}

func TestChinwagClone(t *testing.T) {
  seuss := OpenEmbedded("Seussian")
  file_name := "chinwag_testcase_seuss_dict_to_s"
  testcase, err := ioutil.ReadFile(path.Join("testcases", file_name))
  if err != nil { log.Fatal(err) }

  clone := seuss.Clone()

  if seuss.String() != string(testcase) || clone.String() != string(testcase) {
    t.Error("\"seuss\" and \"clone\" don't match the testcase")
  }

  seuss.Close()

  if seuss.String() != "[]" {
    t.Error("\"seuss\" couldn't be closed")
  }

  if clone.String() != string(testcase) {
    t.Error("\"seuss\" and \"clone\" were linked")
  }

  seuss = clone.Dup()

  if seuss.String() != string(testcase) || clone.String() != string(testcase) {
    t.Error("\"seuss\" and \"clone\" don't match the testcase")
  }

  clone.Close()

  if seuss.String() != string(testcase) {
    t.Error("\"seuss\" and \"clone\" were linked")
  }

  if clone.String() != "[]" {
    t.Error("\"clone\" couldn't be closed")
  }
}

func TestChinwagPrune(t *testing.T) {
  flooder := []string{"this", "is", "a", "string", "that", "will", "be",
  "duplicated"}

  file_name_1 := "chinwag_testcase_prune_original"
  file_name_2 := "chinwag_testcase_prune_pruned"

  testcase_original,err_1 := ioutil.ReadFile(path.Join("testcases",file_name_1))
  if err_1 != nil { log.Fatal(err_1) }
  testcase_pruned,err_2 := ioutil.ReadFile(path.Join("testcases", file_name_2))
  if err_2 != nil { log.Fatal(err_2) }

  small_mess := Open()

  small_mess.PlaceSlice(flooder)
  small_mess.PlaceSlice(flooder)
  small_mess.PlaceSlice(flooder)

  if small_mess.String() != string(testcase_original) {
    t.Error("flooded \"small_mess\" does not equal testcase")
  }

  small_mess.Prune()

  if small_mess.String() != string(testcase_pruned) {
    t.Error("flooded and pruned \"small_mess\" does not equal testcase")
  }
}

func TestChinwagClean(t *testing.T) {
  flooder := []string{"this", "is", "a", "string", "that", "will", "be",
  "duplicated"}

  file_name_1 := "chinwag_testcase_clean_original"
  file_name_2 := "chinwag_testcase_clean_cleaned"

  testcase_original,err_1 := ioutil.ReadFile(path.Join("testcases",file_name_1))
  if err_1 != nil { log.Fatal(err_1) }
  testcase_pruned,err_2 := ioutil.ReadFile(path.Join("testcases", file_name_2))
  if err_2 != nil { log.Fatal(err_2) }

  small_mess := Open()

  small_mess.PlaceSlice(flooder)
  small_mess.PlaceSlice(flooder)
  small_mess.PlaceSlice(flooder)

  if small_mess.String() != string(testcase_original) {
    t.Error("flooded \"small_mess\" does not equal testcase")
  }

  small_mess.Clean()

  if small_mess.String() != string(testcase_pruned) {
    t.Error("flooded and cleaned \"small_mess\" does not equal testcase")
  }
}

func TestChinwagSample(t *testing.T) {
  seuss := OpenEmbedded("Seussian")
  var sample string = seuss.Sample()

  if seuss.Exclude(sample) {
    t.Error("expected \"seuss\" to include sample (%s)", sample)
  }
}

func TestChinwagValidate(t *testing.T) {
  seuss := OpenEmbedded("Seussian")
  small_mess := Open()
  small_mess.PlaceSlice([]string{"this", "is", "a", "quick", "test", "of",
  "validity"})

  e_1 := seuss.Validate()
  e_2 := small_mess.Validate()

  if e_1 != nil {
    t.Error("\"seuss\" is invalid (for some dumb reason)")
  }

  if e_2 == nil {
    t.Error("\"small_mess\" validation doesn't fail as expected")
  }

  for i := 0; i != 300; i++ {
    small_mess.PlaceSlice([]string{"more", "test", "words"})
  }

  if small_mess.Length() < 300 {
    t.Error("expected >300, got %d", small_mess.Length())
  }

  e_3 := small_mess.Validate()

  if e_3 == nil {
    t.Error("\"small_mess\" validation doesn't fail as expected")
  }
}

func TestChinwagIncludeExclude(t *testing.T) {
  include_a := []string{"this", "is", "a", "test", "of", "include"}
  exclude_a := []string{"we", "are", "now", "testing", "exclude"}

  small_mess := Open()
  small_mess.PlaceSlice(include_a)

  for _, w := range include_a {
    if !small_mess.Include(w) {
      t.Errorf("expected \"small_mess\" to include \"%s\"", w)
    }

    if small_mess.Exclude(w) {
      t.Errorf("expected \"small_mess\" to not exclude \"%s\"", w)
    }
  }

  for _, w := range exclude_a {
    if small_mess.Include(w) {
      t.Errorf("expected \"small_mess\" to not include \"%s\"", w)
    }

    if !small_mess.Exclude(w) {
      t.Errorf("expected \"small_mess\" to exclude \"%s\"", w)
    }
  }
}

func TestChinwagString(t *testing.T) {
  seuss := OpenEmbedded("Seussian")
  file_name := "chinwag_testcase_seuss_dict_to_s"
  testcase, err := ioutil.ReadFile(path.Join("testcases", file_name))
  if err != nil { log.Fatal(err) }

  if seuss.String() != string(testcase) {
    t.Error("expected \"seuss\" to equal testcase")
  }
}

func TestChinwagConcatenation(t *testing.T) {
  seuss := OpenEmbedded("Seussian")

  seuss.PlaceSlice([]string{"abcdefg", "hijklmn"})

  if seuss.Exclude("abcdefg") || seuss.Exclude("hijklmn") {
    t.Error("expected \"seuss\" to include \"abcedfg\" and \"hijklmn\"")
  }

  seuss.PlaceWord("oqrstuv")

  if seuss.Exclude("oqrstuv") {
    t.Error("expected \"seuss\" to include \"oqrstuv\"")
  }

  small_mess := Open()

  if small_mess.String() != "[]" {
    t.Error("expected \"small_mess\" to be empty")
  }

  small_mess.PlaceWords("this", "is", "a", "quick", "test")

  if small_mess.Exclude("this") || small_mess.Exclude("is") ||
  small_mess.Exclude("a") || small_mess.Exclude("quick") ||
  small_mess.Exclude("test") {
    t.Error("\"small_mess\" doesn't include required elements")
  }
}

func TestChinwagEquality(t *testing.T) {
  empty := Open()
  seuss := OpenEmbedded("Seussian")

  slight := empty.Clone(); slight.PlaceWords("some", "test", "entries")

  if slight == empty {
    t.Error("expected \"slight\" to not equal \"empty\"")
  }

  if slight == seuss {
    t.Error("expected \"slight\" to not equal \"seuss\"")
  }

  empty = seuss.Clone(); empty.Close()
  if empty == seuss {
    t.Error("expected \"empty\" to not equal \"seuss\"")
  }

  shallow := seuss
  if shallow != seuss {
    t.Error("expected \"shallow\" to equal \"seuss\"")
  }

  shallow = seuss.Clone()
  if shallow == seuss {
    t.Error("expected \"shallow\" not to equal \"seuss\"")
  }
}
