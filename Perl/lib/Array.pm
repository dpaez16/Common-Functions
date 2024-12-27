=head1  Description

Wrapper for array data structure.

=cut

package Array;

=head2 new

    Creates a new array instance. Can optionally pass in an array if you want to pre-initialize it.

    Usage:
        my $arr_ref = [1, 5, 7, "str", 100];
        
        my $arr = new Array();
        my $arr_alt = new Array($arr_ref);

        my $arr_from_Array = new Array($arr);

    Parameters:
        - $elements : array reference | Array | HashSet
            If passed in, the array's contents will be initialized with the elements populated in $elements.

    Returns:
        - The array instance
=cut

sub new {
	my ($class, $elements) = @_;
	my $self = {
		arr => []
	};

	bless $self, $class;

	if (defined $elements) {
        if (ref($elements) eq "Array") {
            $elements->forEach(sub {
                my $elem = shift;
                $self->push($elem);
            });
        } elsif (ref($elements) eq "HashSet") {
            $elements->forEach(sub {
                my $elem = shift;
                $self->push($elem);
            });
        } elsif (ref($elements) eq "ARRAY") {
            # we're passing in a Perl array reference
            for my $elem (@$elements) {
                $self->push($elem);
            }
        }
	}

	return $self;
};

=head2 fromPerlArray

    Creates a new array instance from a Perl array.

    Usage:
        my @arr = (1, 5, 7, "str", 100);
        my $arr_from_perl_array = Array::fromPerlArray(@arr);

    Parameters:
        - @perlArray : array
            The Perl array that will be used to populate the new Array instance.

    Returns:
        - The array instance.
=cut

sub fromPerlArray {
    my (@perlArray) = @_;
    return new Array(\@perlArray);
};

=head2 push

    Appends an element to the end of the array.

    Usage:
        $arr->push($elem);

    Parameters:
        - $elem : any
            Element that will be appended.

    Returns:
        - Nothing.
=cut

sub push {
	my ($self, $elem) = @_;
	my $arr = $self->{arr};

	CORE::push(@{$arr}, $elem);
};

=head2

    Executes a function over the array's elements.

    Usage:
        my $closure = sub { ... };

        $arr->forEach(sub { ... });
        $arr->forEach($closure);

    Parameters:
        - $func : sub
            Function that will be used on the array's elements.
            Arguments: $elem, $idx

    Returns:
        - Nothing.

=cut

sub forEach {
    my ($self, $func) = @_;

    for (my $idx = 0; $idx < $self->size(); $idx++) {
        my $elem = $self->get($idx);
        $func->($elem, $idx);
    }
};

=head2 find

    Searches the array for the first element that satisfies the provided function.

    Usage:
        my $closure = sub { ... };

        my $found_elem = $arr->find(sub { ... });
        my $found_elem2 = $arr->find($closure);

    Parameters:
        - $func : sub
            Function that will be used against the array's elements.

    Returns:
        - The first element that satisfies the given function, otherwise undef.
=cut

sub find {
	my ($self, $func) = @_;
	
	for (my $idx = 0; $idx < $self->size(); $idx++) {
		my $elem = $self->get($idx);
		if ($func->($elem, $idx)) {
			return $elem;
		}
	}

	return undef;
};

=head2 findIndex

    Searches the array for the first element that satisfies the provided function and returns it's index.

    Usage:
        my $closure = sub { ... };

        my $found_elem_idx = $arr->findIndex(sub { ... });
        my $found_elem_idx2 = $arr->findIndex($closure);

    Parameters:
        - $func : sub
            Function that will be used against the array's elements.

    Returns:
        - The index of the first element that satisfies the given function, otherwise -1.
=cut

sub findIndex {
	my ($self, $func) = @_;
	
	for (my $idx = 0; $idx < $self->size(); $idx++) {
		my $elem = $self->get($idx);
		if ($func->($elem, $idx)) {
			return $idx;
		}
	}

	return -1;
};

=head2 findLastIndex

    Searches the array in reverse order for the first element that satisfies the provided function and returns it's index.

    Usage:
        my $closure = sub { ... };

        my $found_elem_idx = $arr->findLastIndex(sub { ... });
        my $found_elem_idx2 = $arr->findLastIndex($closure);

    Parameters:
        - $func : sub
            Function that will be used against the array's elements.

    Returns:
        - The index of the first element that satisfies the given function, otherwise -1.
=cut

sub findLastIndex {
	my ($self, $func) = @_;
	
	for (my $idx = $self->size() - 1; $idx >= 0; $idx--) {
		my $elem = $self->get($idx);
		if ($func->($elem, $idx)) {
			return $idx;
		}
	}

	return -1;
};

=head2 filter

    Creates a new array that consists of only the elements that satisfy the provided function.

    Usage:
        my $closure = sub { ... };

        my $filtered_elem = $arr->filter(sub { ... });
        my $filtered_elem2 = $arr->filter($closure);

    Parameters:
        - $func : sub
            Function that will be used against the array's elements.
            Arguments: $elem, $idx

    Returns:
        - The filtered array.
=cut

sub filter {
	my ($self, $func) = @_;

	my $filtered_arr = new Array();
	for (my $idx = 0; $idx < $self->size(); $idx++) {
		my $elem = $self->get($idx);
		if ($func->($elem, $idx)) {
			$filtered_arr->push($elem);
		}
	}

	return $filtered_arr;
};

=head2 join
    Joins all elements of the array into a string.

    Usage:
        my $joined_str = $arr->join(",");

    Parameters:
        - $separator : string
            Specifies a string to separate each element of the array.

    Returns:
        - The joined string.
=cut

sub join {
    my ($self, $separator) = @_;
    my $arr = $self->{arr};

    return CORE::join($separator, @$arr);
};

=head2 map

    Creates a new array that consists of the elements after applying a function to each of them.

    Usage:
        my $closure = sub { ... };

        my $mapped_arr = $arr->map(sub { ... });
        my $mapped_arr2 = $arr->map($closure);

    Parameters:
        - $func : sub
            Function that will be used on the array's elements.
            Arguments: $elem, $idx

    Returns:
        - The mapped array.
=cut

sub map {
    my ($self, $func) = @_;

    my $mapped_arr = new Array();
    for (my $idx = 0; $idx < $self->size(); $idx++) {
        my $elem = $self->get($idx);
        $mapped_arr->push($func->($elem, $idx));
    }

    return $mapped_arr;
};

=head2 get

    Gets the element of the array at a provided index.

    Usage:
        my $elem = $arr->get($idx);

    Parameters:
        - $idx : number
            Index of the array to grab from.

    Returns:
        - The element of the array at the index. Will return undef if the index is invalid.
=cut

sub get {
	my ($self, $idx) = @_;
	my $arr = $self->{arr};

	return $arr->[$idx];
};

=head2 last

    Gets the last element of the array.

    Usage:
        my $elem = $arr->last();

    Returns:
        - The last element of the array. Will return undef if the array is empty.
=cut

sub last {
    my ($self) = @_;
    return $self->get(-1);
};

=head2 reduce

    Executes a user-supplied "reducer" callback function on
    each element of the array, in order, passing in the return
    value from the calculation on the preceding element.
    The final result of running the reducer across all
    elements of the array is a single value.

    Usage:
        my $closure = sub { ... };

        my $reduced = $arr->reduce(sub { ... });
        my $reduced2 = $arr->reduce($closure);
        my $reduced3 = $arr->reduce(sub { ... }, $initial_value);

    Parameters:
        - $func : sub
            The "reducer" function that will be used against the array's elements.
            Arguments: $acc, $current_value, $current_idx
        - $initial_value : any
            Optional. The initial value that will be used as the accumulator.
            If not supplied, the first element of the array will be used.

    Returns:
        - The reduced value.

=cut

sub reduce {
    my ($self, $func, $initial_value) = @_;
    my $initial_idx = defined($initial_value) ? 0 : 1;
    my $acc = defined($initial_value) ? $initial_value : $self->get(0);

    for (my $idx = $initial_idx; $idx < $self->size(); $idx++) {
        my $elem = $self->get($idx);
        $acc = $func->($acc, $elem, $idx);
    }

    return $acc;
};

=head2 reverse

    Reverses the array in place.

    Usage:
        $arr->reverse();

    Returns:
        - Nothing.
=cut

sub reverse {
    my ($self) = @_;
    my $arr = $self->{arr};
    @{$self->{arr}} = CORE::reverse @$arr;
};

=head2 set

    Assigns the provided element to the array at a specified index.

    Usage:
        $arr->set($idx, $elem);

    Parameters:
        - $idx : number
            Index of the array.
        - $elem : any
            Element that will be placed in the array.

    Returns:
        - Nothing.
=cut

sub set {
	my ($self, $idx, $elem) = @_;
	my $arr = $self->{arr};

	$arr->[$idx] = $elem;
};

=head2 size

    Gets the number of elements in the array.

    Usage:
        my $arr_size = $arr->size();

    Returns:
        - The array size.
=cut

sub size {
	my $self = shift;
	my $arr = $self->{arr};

	return CORE::scalar(@{$arr});
};

=head2 some

    Tests whether at least one element in the array passes the test implemented by the provided function.

    Usage:
        my $closure = sub { ... };

        my $some = $arr->some(sub { ... });
        my $some2 = $arr->some($closure);

    Parameters:
        - $func : sub
            Function that will be used against the array's elements.
            Arguments: $elem, $idx

    Returns:
        - 1 if the function returns a truthy value for any element in the array, 0 otherwise.
=cut

sub some {
    my ($self, $func) = @_;
    return defined $self->find($func) ? 1 : 0;
};

=head2 all

    Tests whether all elements in the array passes the test implemented by the provided function.

    Usage:
        my $closure = sub { ... };

        my $val = $arr->all(sub { ... });
        my $val2 = $arr->all($closure);

    Parameters:
        - $func : sub
            Function that will be used against the array's elements.
            Arguments: $elem, $idx

    Returns:
        - 1 if the function returns a truthy value for all elements in the array, 0 otherwise.
=cut

sub all {
    my ($self, $func) = @_;
    for (my $idx = 0; $idx < $self->size(); $idx++) {
        my $elem = $self->get($idx);
        if (!$func->($elem, $idx)) {
            return 0;
        }
    }

    return 1;
};

=head2 concat

    Concatenates an array to the current array.
    This method does not change the existing arrays, but instead returns a new array.

    Usage:
        my $concat_arr = $arr->concat($other_arr);

    Parameters:
        - $other_arr : Array
            The array to merge with the current array.

    Returns:
        - The merged array.

=cut

sub concat {
    my ($self, $other_arr) = @_;

    my $new_arr = new Array($self);
    $other_arr->forEach(sub {
        my $elem = shift;
        $new_arr->push($elem);
    });

    return $new_arr;
}

=head2 sort

    Sorts the array in place.

    Usage:
        $arr->sort();
        $arr->sort(sub {
            my ($a, $b) = @_;
            return ...;
        });

    Parameters:
        - $func : sub
            Optional. A function that defines the sort order. It must return a value.
            Arguments: $a, $b

    Returns:
        - Nothing.
=cut

sub sort {
    my ($self, $func) = @_;
    my $arr = $self->{arr};

    if (defined $func) {
        @{$self->{arr}} = CORE::sort { $func->($a, $b) } @$arr;
    } else {
        @{$self->{arr}} = CORE::sort @$arr;
    }
};

=head2 slice
    Returns a sub-array of the array from $start to $end (exclusive).

    Usage:
        my $sliced_arr = $arr->slice($start, $end);
        my $other_sliced_arr = $arr->slice($start);

    Parameters:
        - $start : number
            The starting index of the specified portion of the array.
        - $end : number
            Optional. The end of the specified portion of the array. If left out, this is assumed to be the end of the array.

    Returns:
        - The sliced Array.

=cut

sub slice {
    my ($self, $start, $end) = @_;
    my $sliced = new Array();

    if (! defined $end) {
        $end = $self->size();
    }

    for (my $idx = $start; $idx < $end; $idx++) {
        $sliced->push($self->get($idx));
    }

    return $sliced;
};

=head2 splice

    Changes the contents of the array by removing or replacing existing elements and/or adding new elements in place.

    Usage:
        $arr->splice($start, $delete_count, $elements);

    Parameters:
        - $start : number
            Index at which to start changing the array.
        - $delete_count : number
            An integer indicating the number of old array elements to remove.
        - $elements : Array | array reference
            The elements to add to the array, beginning at the start index.

    Returns:
        - An array of the elements that were removed/replaced.
=cut

sub splice {
    my ($self, $start, $delete_count, $elements) = @_;
    my $arr = $self->{arr};

    $start = 0 if ! defined $start;
    $delete_count = $self->size() if ! defined $delete_count;

    if (!defined $elements) {
        my @splicedElements = CORE::splice(@{$arr}, $start, $delete_count);
        return new Array(\@splicedElements);
    }

    if (ref($elements) eq "Array") {
        $elements = $elements->to_raw_array_ref();
    }

    my @splicedElements = CORE::splice(@{$arr}, $start, $delete_count, @$elements);
    return Array::fromPerlArray(@splicedElements);
};

=head2 toReversed

    Returns a reversed copy of the array.

    Usage:
        my $reversed_arr = $arr->toReversed();

    Returns:
        - The reversed Array.
=cut

sub toReversed {
    my ($self) = @_;
    my $reversed_arr = new Array($self);
    $reversed_arr->reverse();
    return $reversed_arr;
};

=head2 toSorted

    Returns a sorted copy of the array.

    Usage:
        my $sorted_arr = $arr->toSorted();
        my $other_sorted_arr = $arr->toSorted(sub {
            my ($a, $b) = @_;
            return ...;
        });

    Parameters:
        - $func : sub
            Optional. A function that defines the sort order. It must return a value.
            Arguments: $a, $b

    Returns:
        - The sorted Array.
=cut

sub toSorted {
    my ($self, $func) = @_;

    my $sorted_arr = new Array($self);
    $sorted_arr->sort($func);
    return $sorted_arr;
};

=head2 to_string

    Gets a string representation of the array.

    Usage:
        my $arr_str = $arr->to_string();

    Parameters:
        - $no_brackets : boolean
            If true, the string will not include brackets.

    Returns:
        - The string representation of the array.
=cut

sub to_string {
	my $self = shift;
    my $no_brackets = shift;

	my $arr = $self->{arr};

    if ($no_brackets) {
        return CORE::join(", ", @{$arr});
    }

	return "[" . CORE::join(", ", @{$arr}) . "]";
};

=head2 to_raw_array

    Gets the raw representation of the array (as a copy).

    Usage:
        my $arr = new Array();
        ...
        my @raw_arr = $arr->to_raw_array();

    Returns:
        - The raw representation of the array.
=cut

sub to_raw_array {
    my $self = shift;
    my $arr = $self->{arr};

    return @$arr;
};

=head2 to_raw_array_ref

    Gets the raw representation of the array (as a copy ref).

    Usage:
        my $arr = new Array();
        ...
        my $raw_arr_ref = $arr->to_raw_array_ref();

    Returns:
        - The raw representation of the array (as an array ref).
=cut

sub to_raw_array_ref {
    my $self = shift;
    my $arr = $self->{arr};
    my @arr_copy = map { $_ } @$arr;

    return \@arr_copy;
};

1;
