#!/usr/bin/perl

use strict;
use warnings;
use diagnostics;
use Test::More;

use Array;
use HashMap;

test_hashmap_construction();
test_hashmap_get_set_delete();
test_hashmap_clear();
test_hashmap_has();
test_hashmap_keys_values_entries();
test_hashmap_to_raw_hash();
test_hashmap_to_raw_hash_ref();
test_hashmap_to_string();
test_hashmap_independence();

done_testing();

sub test_hashmap_construction {
    my $dict = new HashMap();
    is($dict->size(), 0, qq{dict.size == 0});

    my %perlHash = ('a' => 1, 'b' => 'c');
    my $dict_from_hashref = new HashMap(\%perlHash);
    is($dict_from_hashref->size(), 2, qq{dict_from_hashref.size == 2});
    is($dict_from_hashref->has('a'), 1, qq{dict_from_hashref.has('a') == true});
    is($dict_from_hashref->has('b'), 1, qq{dict_from_hashref.has('b') == true});
    is($dict_from_hashref->has('c'), 0, qq{dict_from_hashref.has('c') == false});
    is($dict_from_hashref->get('a'), 1, qq{dict_from_hashref.get('a') == 1});
    is($dict_from_hashref->get('b'), 'c', qq{dict_from_hashref.get('b') == 'c'});
    is($dict_from_hashref->get('c'), undef, qq{dict_from_hashref.get('c') == undefined});

    my $perlHashRef = {'d' => 0, 'e' => 1,};
    $dict_from_hashref = new HashMap($perlHashRef);
    is($dict_from_hashref->size(), 2, qq{dict_from_hashref.size == 2});
    is($dict_from_hashref->has('d'), 1, qq{dict_from_hashref.has('d') == true});
    is($dict_from_hashref->has('e'), 1, qq{dict_from_hashref.has('e') == true});
    is($dict_from_hashref->has('f'), 0, qq{dict_from_hashref.has('f') == false});
    is($dict_from_hashref->get('d'), 0, qq{dict_from_hashref.get('d') == 0});
    is($dict_from_hashref->get('e'), 1, qq{dict_from_hashref.get('e') == 1});
    is($dict_from_hashref->get('f'), undef, qq{dict_from_hashref.get('f') == undefined});

    my $dict_from_dict = new HashMap($dict_from_hashref);
    is($dict_from_dict->size(), 2, qq{dict_from_dict.size == 2});
    is($dict_from_dict->has('d'), 1, qq{dict_from_dict.has('d') == true});
    is($dict_from_dict->has('e'), 1, qq{dict_from_dict.has('e') == true});
    is($dict_from_dict->has('f'), 0, qq{dict_from_dict.has('f') == false});
    is($dict_from_dict->get('d'), 0, qq{dict_from_dict.get('d') == 0});
    is($dict_from_dict->get('e'), 1, qq{dict_from_dict.get('e') == 1});
    is($dict_from_dict->get('f'), undef, qq{dict_from_dict.get('f') == undefined});

    $dict_from_dict = new HashMap($dict);
    is($dict_from_dict->size(), 0, qq{dict_from_dict.size == 0});
}

sub test_hashmap_get_set_delete {
    my $dict = new HashMap();
    $dict->set('a', 1);
    $dict->set('b', 'c');
    $dict->set('d', 0);

    is($dict->size(), 3, qq{dict.size == 3});
    is($dict->get('a'), 1, qq{dict.get('a') == 1});
    is($dict->get('b'), 'c', qq{dict.get('b') == 'c'});
    is($dict->get('d'), 0, qq{dict.get('d') == 0});

    my $val = $dict->delete('b');
    is($dict->size(), 2, qq{dict.size == 2});
    is($val, 1, qq{dict.delete('b') == true});
    is($dict->has('b'), 0, qq{dict.has('b') == false});
    is($dict->get('b'), undef, qq{dict.get('b') == undefined});

    $val = $dict->delete('f');
    is($dict->size(), 2, qq{dict.size == 2});
    is($val, 0, qq{dict.delete('f') == true});
    is($dict->has('f'), 0, qq{dict.has('f') == false});
    is($dict->get('f'), undef, qq{dict.get('f') == undefined});
}

sub test_hashmap_clear {
    my $hashRef = {
        a => 1,
        b => 2,
        c => 3,
    };
    my $hashRefSize = scalar(%$hashRef);
    my $dict = new HashMap($hashRef);

    is($dict->size(), $hashRefSize, qq{dict.size == $hashRefSize});

    $dict->clear();
    is($dict->size(), 0, qq{dict.size == 0});
    is($dict->has('a'), 0, qq{dict.has('a') == false});
    is($dict->has('b'), 0, qq{dict.has('b') == false});
    is($dict->has('c'), 0, qq{dict.has('c') == false});
}

sub test_hashmap_has {
    my $dict = new HashMap({
        a => undef,
    });

    is($dict->has('a'), 1, qq{dict.has('a') == true});
}

sub test_hashmap_keys_values_entries {
    my $dict = new HashMap();
    $dict->set('a', 1);
    $dict->set('b', 2);

    my $keys = Array::fromPerlArray($dict->keys());
    is($keys->size(), 2, qq{dict.keys.size == 2});

    my $valA = $keys->some(sub { my $elem = shift; return $elem eq 'a'; });
    my $valB = $keys->some(sub { my $elem = shift; return $elem eq 'b'; });
    is($valA, 1, qq{keys.some(e => e == 'a') == true});
    is($valB, 1, qq{keys.some(e => e == 'b') == true});

    my $values = Array::fromPerlArray($dict->values());
    is($values->size(), 2, qq{dict.values.size == 2});

    $valA = $values->some(sub { my $elem = shift; return $elem eq '1'; });
    $valB = $values->some(sub { my $elem = shift; return $elem eq '2'; });
    is($valA, 1, qq{values.some(e => e == '1') == true});
    is($valB, 1, qq{values.some(e => e == '2') == true});

    my $entries = Array::fromPerlArray($dict->entries());
    is($entries->size(), 2, qq{dict.entries.size == 2});
    $entries->forEach(sub {
        my ($entry, $idx) = @_;
        is(ref($entry), 'ARRAY', qq{typeof(entries[$idx]) == 'ARRAY'});

        my $arr = new Array($entry);
        is($arr->size(), 2, qq{entries[$idx].size == 2});
    });

    my $val = $entries->some(sub {
        my $entry = shift;
        my $arr = new Array($entry);
        return $arr->get(0) eq 'a';
    });
    is($val, 1, qq{entries.some(e => e[0] == 'a')});

    $val = $entries->some(sub {
        my $entry = shift;
        my $arr = new Array($entry);
        return $arr->get(0) eq 'b';
    });
    is($val, 1, qq{entries.some(e => e[0] == 'b')});
}

sub test_hashmap_to_raw_hash {
    my $dict = new HashMap({
        a => 1,
        b => 2,
    });

    my %perlHash = $dict->to_raw_hash();
    is(scalar(%perlHash), 2, qq{dict.to_raw_hash().size == 2});
    is($perlHash{'a'}, 1, qq{perlHash.get('a') == 1});
    is($perlHash{'b'}, 2, qq{perlHash.get('b') == 2});
}

sub test_hashmap_to_raw_hash_ref {
    my $dict = new HashMap({
        a => 1,
        b => 2,
    });

    my $perlHashRef = $dict->to_raw_hash_ref();
    is(scalar(%$perlHashRef), 2, qq{dict.to_raw_hash_ref().size == 2});
    is($perlHashRef->{'a'}, 1, qq{perlHashRef.get('a') == 1});
    is($perlHashRef->{'b'}, 2, qq{perlHashRef.get('b') == 2});
}

sub test_hashmap_to_string {
    my $dict = new HashMap({
        a => 1,
    });

    my $expected = qq{\{(a,1)\}};
    is($dict->to_string(), $expected, qq{dict.to_string() == '$expected'});
}

sub test_hashmap_independence {
    my $dict1 = new HashMap();
    my $dict2 = new HashMap();

    $dict1->set('a', 1);
    $dict1->set('b', 2);
    $dict1->set('c', 3);

    $dict2->set('c', 4);
    $dict2->set('d', 5);

    is($dict1->size(), 3, qq{dict1.size() == 3});
    is($dict1->get('a'), 1, qq{dict1.get('a') == 1});
    is($dict1->get('b'), 2, qq{dict1.get('b') == 2});
    is($dict1->get('c'), 3, qq{dict1.get('c') == 3});

    is($dict2->size(), 2, qq{dict2.size() == 2});
    is($dict2->get('c'), 4, qq{dict2.get('c') == 4});
    is($dict2->get('d'), 5, qq{dict2.get('d') == 5});

    $dict2->delete('c');

    is($dict1->size(), 3, qq{dict1.size() == 3});
    is($dict1->get('c'), 3, qq{dict1.get('c') == 3});
    is($dict2->size(), 1, qq{dict2.size() == 1});
    is($dict2->has('c'), 0, qq{dict2.has('c') == false});
}