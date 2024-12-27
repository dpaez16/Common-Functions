=head1  Description

Wrapper for set data structure.

=cut

package HashSet;

=head2 new

    Creates a new set instance. Can optionally pass in an array if you want to pre-initialize it.
    It is assumed that elements passed in are easily hashable (i.e. primitive values).

    Usage:
        my $arr_ref = [1, 5, 7, "str", 100];
        
        my $set = new HashSet();
        my $set_alt = new HashSet($arr_ref);

    Parameters:
        - $elements : array reference | Array | HashSet
            If passed in, the array's contents will be initialized with the elements populated in $elements.

    Returns:
        - The set instance
=cut

sub new {
    my ($class, $elements) = @_;
    my $self = {
        set => {}
    };

    bless $self, $class;

    if (defined $elements) {
        if (ref($elements) eq "Array") {
            $elements->forEach(sub {
                my $elem = shift;
                $self->add($elem);
            });
        } elsif (ref($elements) eq "HashSet") {
            $elements->forEach(sub {
                my $elem = shift;
                $self->add($elem);
            });
        } elsif (ref($elements) eq "ARRAY") {
            # we're passing in a regular Perl array reference
            for my $elem (@$elements) {
                $self->add($elem);
            }
        }
    }

    return $self;
};

=head2 add

    Adds an element to the set.

    Usage:
        $set->add(5);
        $set->add("str");

    Parameters:
        - $elem : any
            The element to add to the set.

    Returns:
        - None
=cut

sub add {
    my ($self, $elem) = @_;
    my $set = $self->{set};

    $set->{$elem} = undef;
};

=head2 remove

    Removes an element from the set.

    Usage:
        $set->remove(5);
        $set->remove("str");

    Parameters:
        - $elem : any
            The element to remove from the set.

    Returns:
        - None
=cut

sub remove {
    my ($self, $elem) = @_;
    my $set = $self->{set};

    delete($set->{$elem});
};

=head2 contains

    Checks if the set contains the given element.

    Usage:
        my $result = $set->contains(5);
        my $result_alt = $set->contains("str");

    Parameters:
        - $elem : any
            The element to check for in the set.

    Returns:
        - 1 if the set contains the element, 0 otherwise.
=cut

sub contains {
    my ($self, $elem) = @_;
    my $set = $self->{set};

    return exists($set->{$elem}) ? 1 : 0;
};

=head2 size

    Returns the number of elements in the set.

    Usage:
        my $size = $set->size();

    Returns:
        - The number of elements in the set.
=cut

sub size {
    my $self = shift;
    my $set = $self->{set};

    return scalar(CORE::keys(%$set));
};

=head2 empty

    Checks if the set is empty.

    Usage:
        my $result = $set->empty();

    Returns:
        - True if the set is empty, false otherwise.
=cut

sub empty {
    my $self = shift;
    return $self->size() == 0;
};

=head2

    Executes a function over the set's elements.

    Usage:
        my $closure = sub { ... };

        $arr->forEach(sub { ... });
        $arr->forEach($closure);

    Parameters:
        - $func : sub
            Function that will be used on the set's elements.
            Arguments: $elem

    Returns:
        - Nothing.

=cut

sub forEach {
    my ($self, $func) = @_;
    my $set = $self->{set};

    for my $elem (CORE::keys(%$set)) {
        $func->($elem);
    }
};

=head2 minus

    Returns a new set that is the difference between the current set and another set.

    Usage:
        my $set_diff = $set->minus($other_set); # equivalent to $set - $other_set

    Parameters:
        - $other_set : HashSet
            The set to subtract from the current set.

    Returns:
        - A new set that is the difference between the current set and the other set.

=cut

sub minus {
    my ($self, $other_set) = @_;

    my $set = $self->{set};
    my $new_set = new HashSet();

    for my $elem (CORE::keys(%$set)) {
        if (!$other_set->contains($elem)) {
            $new_set->add($elem);
        }
    }

    return $new_set;
};

=head2 union

    Returns a new set that is the union between the current set and another set.

    Usage:
        my $set_union = $set->union($other_set); # equivalent to $set + $other_set

    Parameters:
        - $other_set : HashSet
            The set to add with the current set.

    Returns:
        - A new set that is the union between the current set and the other set.

=cut

sub union {
    my ($self, $other_set) = @_;

    my $new_set = new HashSet($self);

    $other_set->forEach(sub {
        my $elem = shift;
        $new_set->add($elem);
    });

    return $new_set;
};

=head2 intersection

    Returns a new set that is the intersection between the current set and another set.

    Usage:
        my $set_union = $set->intersection($other_set);

    Parameters:
        - $other_set : HashSet
            The set to intersect with the current set.

    Returns:
        - A new set that is the intersection between the current set and the other set.

=cut

sub intersection {
    my ($self, $other_set) = @_;

    my $new_set = new HashSet();
    my $smaller_set = $self->size() > $other_set->size() ? $other_set : $self;

    my $closure = sub {
        my $elem = shift;

        if ($self->contains($elem) && $other_set->contains($elem)) {
            $new_set->add($elem);
        }
    };

    $smaller_set->forEach($closure);

    return $new_set;
};

=head2 pop
    Removes an element from the set.

    Usage:
        my $item = $set->pop();

    Returns:
        An item from the set.
        undef is returned if the set is empty.
=cut

sub pop {
    my ($self) = @_;
    my $set = $self->{set};

    if ($self->empty()) {
        return undef;
    }

    my $item = undef;
    for my $elem (CORE::keys(%$set)) {
        $item = $elem;
        last;
    }

    $self->remove($item);
    return $item;
};

=head2 to_raw_array

    Gets the raw representation of the set (as a copy).

    Usage:
        my $set = new HashSet();
        ...
        my @raw_arr = $set->to_raw_array();

    Returns:
        - The raw representation of the set.
=cut

sub to_raw_array {
    my $self = shift;
    my $set = $self->{set};

    my $arr = [];
    for my $elem (CORE::keys(%$set)) {
        CORE::push(@$arr, $elem);
    }

    return @$arr;
};

=head2 to_raw_array_ref

    Gets the raw representation of the set (as a copy ref).

    Usage:
        my $set = new HashSet();
        ...
        my $raw_arr_ref = $set->to_raw_array_ref();

    Returns:
        - The raw representation of the set (as an array ref).
=cut

sub to_raw_array_ref {
    my $self = shift;
    my $set = $self->{set};

    my $arr = [];
    for my $elem (CORE::keys(%$set)) {
        CORE::push(@$arr, $elem);
    }

    return $arr;
};

=head2 to_string

    Gets a string representation of the set.

    Usage:
        my $set_str = $set->to_string();

    Parameters:
        - $no_brackets : boolean
            If true, the string will not include brackets.

    Returns:
        - The string representation of the set.
=cut

sub to_string {
    my $self = shift;
    my $no_brackets = shift;

    my $set = $self->{set};

    my $arr = [];
    for my $elem (CORE::keys(%$set)) {
        CORE::push(@$arr, $elem);
    }

    if ($no_brackets) {
        return join(", ", @{$arr});
    }

	return "{" . join(", ", @{$arr}) . "}";
};

1;