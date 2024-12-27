#!/usr/bin/perl

use strict;
use warnings;
use diagnostics;
use Test::More;

use String;
use Array;

test_atoi();
test_to_char_array();
test_trim();

done_testing();

sub test_atoi {
    is(String::atoi("12"), 12, qq{atoi("12") == 12});
    is(String::atoi("-1"), -1, qq{atoi("-1") == -1});
    is(String::atoi("0"),   0, qq{atoi("0") == 0});
    is(String::atoi(""),    0, qq{atoi("") == 0});
    is(String::atoi(undef), 0, qq{atoi(undefined) == 0});
}

sub test_to_char_array {
    my $arr = Array::fromPerlArray(String::to_char_array("abc"));
    is($arr->size(), 3, qq{to_char_array("abc").size == 3});
    is($arr->get(0), "a", qq{arr[0] == "a"});
    is($arr->get(1), "b", qq{arr[1] == "b"});
    is($arr->get(2), "c", qq{arr[2] == "c"});

    $arr = Array::fromPerlArray(String::to_char_array(""));
    is($arr->size(), 0, qq{to_char_array("").size == 0});

    $arr = Array::fromPerlArray(String::to_char_array(undef));
    is($arr->size(), 0, qq{to_char_array(undefined).size == 0});
}

sub test_trim {
    is(String::trim(" 123 "), "123", qq{" 123 ".trim() == "123"});
    is(String::trim(" 123"), "123", qq{" 123".trim() == "123"});
    is(String::trim("123 "), "123", qq{"123 ".trim() == "123"});
}