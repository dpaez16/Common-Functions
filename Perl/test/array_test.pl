#!/usr/bin/perl

use strict;
use warnings;
use diagnostics;
use Test::More;

use Array;

test_array_construction();
test_to_raw_array();
test_to_raw_array_ref();
test_array_get_set_push();
test_array_map();
test_array_filter();
test_array_find();
test_array_reduce();
test_array_reverse();
test_array_sort();
test_array_some();
test_array_slice();
test_array_splice();
test_array_all();
test_array_concat();
test_array_independence();

done_testing();

sub test_array_construction {
    my $arr = new Array();
    is($arr->size(), 0, "arr.size == 0");

    my @perlArray = (1, 2, "a", 300);
    my $arr2 = Array::fromPerlArray(@perlArray);
    is($arr2->size(), scalar(@perlArray), "arr2.size == perlArray.size");
    $arr2->forEach(sub {
        my ($elem, $idx) = @_;
        is($elem, $perlArray[$idx], "arr2[$idx] == perlArray[$idx]");
    });

    my $array_ref = [3, 4, "b", 400, 500];
    my $arr3 = new Array($array_ref);
    is($arr3->size(), scalar(@$array_ref), "arr3.size == array_ref.size");
    $arr3->forEach(sub {
        my ($elem, $idx) = @_;
        is($elem, $array_ref->[$idx], "arr3[$idx] == array_ref[$idx]");
    });

    my $arr4 = new Array($arr2);
    is($arr4->size(), $arr2->size(), "arr4.size == arr2.size");
    $arr4->forEach(sub {
        my ($elem, $idx) = @_;
        is($elem, $arr2->get($idx), "arr4[$idx] == arr2[$idx]");
    });
}

sub test_to_raw_array {
    my $arr = new Array(["a", 1, "def", 200, 300, "c", "d", 10]);
    my @perlArray = $arr->to_raw_array();

    is($arr->size(), scalar(@perlArray), "arr.size == perlArray.size");
    $arr->forEach(sub {
        my ($elem, $idx) = @_;
        is($elem, $perlArray[$idx], "arr[$idx] == perlArray[$idx]");
    });

    my $elem = "test-string";
    $arr->push($elem);
    is($arr->size(), scalar(@perlArray) + 1, "arr.size == perlArray.size + 1");
    is($arr->last(), $elem, qq{arr.last == "$elem"});
    is($perlArray[scalar(@perlArray) - 1], 10, 'perlArray.last == 10');

    CORE::pop(@perlArray);
    is($arr->size(), scalar(@perlArray) + 2, "arr.size == perlArray.size + 2");
    is($perlArray[scalar(@perlArray) - 1], "d", 'perlArray.last == "d"');
    is($arr->last(), $elem, qq{arr.last == "$elem"});
}

sub test_to_raw_array_ref {
    my $arr = new Array(["a", 1, "def", 200, 300, "c", "d", 10]);
    my $perlArrayRef = $arr->to_raw_array_ref();

    is($arr->size(), scalar(@$perlArrayRef), "arr.size == perlArrayRef.size");
    $arr->forEach(sub {
        my ($elem, $idx) = @_;
        is($elem, $perlArrayRef->[$idx], "arr[$idx] == perlArrayRef[$idx]");
    });

    my $elem = "test-string";
    $arr->push($elem);
    is($arr->size(), scalar(@$perlArrayRef) + 1, "arr.size == perlArrayRef.size + 1");
    is($arr->last(), $elem, qq{arr.last == "$elem"});
    is($perlArrayRef->[scalar(@$perlArrayRef) - 1], 10, 'perlArrayRef.last == 10');

    CORE::pop(@$perlArrayRef);
    is($arr->size(), scalar(@$perlArrayRef) + 2, "arr.size == perlArrayRef.size + 2");
    is($perlArrayRef->[scalar(@$perlArrayRef) - 1], "d", 'perlArrayRef.last == "d"');
    is($arr->last(), $elem, qq{arr.last == "$elem"});
}

sub test_array_get_set_push {
    my $arr = new Array();
    $arr->push(1);
    $arr->push("a");
    $arr->push(4);
    $arr->push(-1);

    is($arr->get(0), 1,   qq{arr[0] == 1});
    is($arr->get(1), "a", qq{arr[1] == "a"});
    is($arr->get(2), 4,   qq{arr[2] == 4});
    is($arr->get(3), -1,  qq{arr[3] == -1});

    $arr->set(1, 0);
    is($arr->get(1), 0, qq{arr[1] == 0});
}

sub test_array_map {
    my $arr = new Array([1, 2, 3, 4, 5]);
    my $arr_sq = $arr->map(sub {
        my ($elem) = @_;
        return $elem * $elem;
    });

    $arr_sq->forEach(sub {
        my ($elem_sq, $idx) = @_;
        my $elem = $arr->get($idx);
        is($elem_sq, $elem * $elem, qq{arr[$idx] ** 2 == arr_sq[$idx]});
    });
}

sub test_array_filter {
    my $arr = new Array([1, 2, 3, 4, 5, 6]);
    my $evens = $arr->filter(sub {
        my $elem = shift;
        return $elem % 2 == 0;
    });

    $evens->forEach(sub {
        my ($elem, $idx) = @_;
        is($elem % 2, 0, qq{arr[$idx] % 2 == 0});
    });
}

sub test_array_find {
    my $arr = new Array([1, 2, "a", 4, 5]);
    my $closure = sub {
        my ($elem) = @_;
        return $elem eq "a";
    };

    my $elem = $arr->find($closure);
    my $idx = $arr->findIndex($closure);

    is($elem, "a", qq{arr.find(e => e == "a") == "a"});
    is($idx, 2, qq{arr.findIndex(e => e == "a") == 2});

    $arr->set($idx, 0);

    $elem = $arr->find($closure);
    $idx = $arr->findIndex($closure);

    is($elem, undef, qq{arr.find(e => e == "a") == undefined});
    is($idx, -1, qq{arr.findIndex(e => e == "a") == -1});

    $idx = $arr->findLastIndex(sub {
        my $elem = shift;
        return $elem % 2 == 0;
    });

    is($idx, $arr->size() - 2, qq{arr.findLastIndex(e => e % 2 == 0) == arr.size - 2});
}

sub test_array_reduce {
    my $arr = new Array([1, 2, 3, 4, 5]);
    my $total = $arr->reduce(sub {
        my ($acc, $elem) = @_;
        return $acc + $elem;
    });

    is($total, 15, qq{arr.reduce((acc, e) => acc + e) == 15});

    $total = $arr->reduce(sub {
        my ($acc, $elem) = @_;
        return $acc + $elem;
    }, 0);

    is($total, 15, qq{arr.reduce((acc, e) => acc + e) == 15});

    $total = $arr->reduce(sub {
        my ($acc, $elem) = @_;
        return $acc + $elem;
    }, 10);

    is($total, 25, qq{arr.reduce((acc, e) => acc + e) == 25});
}

sub test_array_reverse {
    my $arr = new Array([1, 2, 3]);
    $arr->reverse();

    is($arr->get(0), 3, qq{arr[0] == 3});
    is($arr->get(1), 2, qq{arr[1] == 2});
    is($arr->get(2), 1, qq{arr[2] == 1});

    $arr->reverse();

    is($arr->get(0), 1, qq{arr[0] == 1});
    is($arr->get(1), 2, qq{arr[1] == 2});
    is($arr->get(2), 3, qq{arr[2] == 3});

    my $arr_rev = $arr->toReversed();

    is($arr_rev->get(0), 3, qq{arr_rev[0] == 3});
    is($arr_rev->get(1), 2, qq{arr_rev[1] == 2});
    is($arr_rev->get(2), 1, qq{arr_rev[2] == 1});

    is($arr->get(0), 1, qq{arr[0] == 1});
    is($arr->get(1), 2, qq{arr[1] == 2});
    is($arr->get(2), 3, qq{arr[2] == 3});
}

sub test_array_sort {
    my $elems = [1, -5, 10, 2];
    my $closure = sub {
        my ($a, $b) = @_;
        return $a <=> $b;
    };

    my $arr = new Array($elems);
    $arr->sort($closure);

    is($arr->get(0), -5, qq{arr[0] == -5});
    is($arr->get(1),  1, qq{arr[1] ==  1});
    is($arr->get(2),  2, qq{arr[2] ==  2});
    is($arr->get(3), 10, qq{arr[3] == 10});

    $arr = new Array($elems);
    my $arr_sorted = $arr->toSorted($closure);

    is($arr->get(0),  1, qq{arr[0] ==  1});
    is($arr->get(1), -5, qq{arr[1] == -5});
    is($arr->get(2), 10, qq{arr[2] == 10});
    is($arr->get(3),  2, qq{arr[3] ==  2});

    is($arr_sorted->get(0), -5, qq{arr_sorted[0] == -5});
    is($arr_sorted->get(1),  1, qq{arr_sorted[1] ==  1});
    is($arr_sorted->get(2),  2, qq{arr_sorted[2] ==  2});
    is($arr_sorted->get(3), 10, qq{arr_sorted[3] == 10});
}

sub test_array_some {
    my $arr = new Array([1, 2, 4, 10]);
    my $closure = sub {
        my $elem = shift;
        return $elem == 10;
    };

    my $val = $arr->some($closure);
    is($val, 1, qq{arr.some(e => e == 10) == true});

    $arr->set($arr->size() - 1, 0);
    $val = $arr->some($closure);
    is($val, 0, qq{arr.some(e => e == 10) == false});
}

sub test_array_slice {
    my $arr = new Array([1, 2, 3, 4, 5, 6]);
    my $sub_arr = $arr->slice(0, 3);

    is($sub_arr->size(), 3, "sub_arr.size == 3");
    $sub_arr->forEach(sub {
        my ($elem, $idx) = @_;
        is($elem, $arr->get($idx), qq{arr[$idx] == sub_arr[$idx]});
    });

    $sub_arr = $arr->slice(3);
    is($sub_arr->size(), 3, "sub_arr.size == 3");
    $sub_arr->forEach(sub {
        my ($elem, $idx) = @_;
        is($elem, $arr->get(3 + $idx), qq{sub_arr[$idx] = arr[3 + $idx]});
    });
}

sub test_array_splice {
    my $arr = new Array([1, 2, 3, 4, 5, 6]);
    my $arr_spliced = $arr->splice(0, 2);

    is($arr->size(), 4, qq{arr.size == 4});
    is($arr->get(0), 3, qq{arr[0] == 3});
    is($arr->get(1), 4, qq{arr[1] == 4});
    is($arr->get(2), 5, qq{arr[2] == 5});
    is($arr->get(3), 6, qq{arr[3] == 6});

    is($arr_spliced->size(), 2, qq{arr.splice(0, 2).size == 2});
    is($arr_spliced->get(0), 1, qq{arr_spliced[0] == 1});
    is($arr_spliced->get(1), 2, qq{arr_spliced[1] == 2});

    $arr_spliced = $arr->splice(0, 2, [7, 8]);
    is($arr_spliced->size(), 2, qq{arr.splice(0, 2, [7, 8]).size == 2});
    is($arr_spliced->get(0), 3, qq{arr_spliced[0] == 3});
    is($arr_spliced->get(1), 4, qq{arr_spliced[1] == 4});

    is($arr->size(), 4, qq{arr.size == 4});
    is($arr->get(0), 7, qq{arr[0] == 7});
    is($arr->get(1), 8, qq{arr[1] == 8});
    is($arr->get(2), 5, qq{arr[2] == 5});
    is($arr->get(3), 6, qq{arr[3] == 6});

    $arr_spliced = $arr->splice(0, 2, Array->new([7, 8]));
    is($arr_spliced->size(), 2, qq{arr.splice(0, 2, [7, 8]).size == 2});
    is($arr_spliced->get(0), 7, qq{arr_spliced[0] == 3});
    is($arr_spliced->get(1), 8, qq{arr_spliced[1] == 4});

    is($arr->size(), 4, qq{arr.size == 4});
    is($arr->get(0), 7, qq{arr[0] == 7});
    is($arr->get(1), 8, qq{arr[1] == 8});
    is($arr->get(2), 5, qq{arr[2] == 5});
    is($arr->get(3), 6, qq{arr[3] == 6});
}

sub test_array_all {
    my $arr = new Array([1, 2, 3, 4, 5]);
    my $closure = sub {
        my $elem = shift;
        return $elem > 0;
    };

    my $val = $arr->all($closure);
    is($val, 1, qq{arr.all(e => e > 0) == true});

    $arr->set(0, -1);
    $val = $arr->all($closure);
    is($val, 0, qq{arr.all(e => e > 0) == false});
}

sub test_array_concat {
    my $arr1 = new Array([1, 2]);
    my $arr2 = new Array([3, 4]);
    my $arr3 = $arr1->concat($arr2);

    is($arr1->size(), 2, qq{arr1.size == 2});
    is($arr1->get(0), 1, qq{arr1[0] == 1});
    is($arr1->get(1), 2, qq{arr1[1] == 2});

    is($arr2->size(), 2, qq{arr1.size == 2});
    is($arr2->get(0), 3, qq{arr2[0] == 3});
    is($arr2->get(1), 4, qq{arr2[1] == 4});

    is($arr3->size(), $arr1->size() + $arr2->size(), qq{arr3.size == arr1.size + arr2.size});
    is($arr3->get(0), $arr1->get(0), qq{arr3[0] == arr1[0]});
    is($arr3->get(1), $arr1->get(1), qq{arr3[1] == arr1[1]});
    is($arr3->get(2), $arr2->get(0), qq{arr3[2] == arr2[0]});
    is($arr3->get(3), $arr2->get(1), qq{arr3[3] == arr2[1]});
}

sub test_array_independence {
    my $arr1 = new Array();
    my $arr2 = new Array();

    $arr1->push(1);
    $arr1->push(2);
    $arr1->push(3);

    $arr2->push(3);
    $arr2->push(4);

    is($arr1->size(), 3, qq{arr1.size == 3});
    is($arr1->get(0), 1, qq{arr1[0] == 1});
    is($arr1->get(1), 2, qq{arr1[1] == 2});
    is($arr1->get(2), 3, qq{arr1[2] == 3});

    is($arr2->size(), 2, qq{arr2.size == 2});
    is($arr2->get(0), 3, qq{arr2[0] == 3});
    is($arr2->get(1), 4, qq{arr2[1] == 4});

    $arr2->set(0, 0);
    $arr2->push(5);
    $arr2->push(6);

    is($arr1->size(), 3, qq{arr1.size == 3});
    is($arr1->get(0), 1, qq{arr1[0] == 1});
    is($arr1->get(1), 2, qq{arr1[1] == 2});
    is($arr1->get(2), 3, qq{arr1[2] == 3});

    is($arr2->size(), 4, qq{arr2.size == 4});
    is($arr2->get(0), 0, qq{arr2[0] == 0});
    is($arr2->get(1), 4, qq{arr2[1] == 4});
    is($arr2->get(2), 5, qq{arr2[2] == 5});
    is($arr2->get(3), 6, qq{arr2[3] == 6});
}