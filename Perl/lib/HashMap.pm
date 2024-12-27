=head1  Description

Wrapper for hash data structure.

=cut

package HashMap;

=head2 new
	Creates a new HashMap instance. Can optionally pass in a hash reference / HashMap if you want to pre-initialize it.

	Usage:
		my $hash_ref = {};
		my %hash = ();

		my $dict = new HashMap();
		my $dict_from_hashref = new HashMap($dict);
		my $dict_from_hash = new HashMap(\%hash);
		my $dict_from_$dict = new HashMap($dict);

	Parameters:
		- $dict : hash reference | HashMap
			If passed in, the HashMap's contents will be initialized with the elements populated in $dict.

	Returns:
		- The HashMap instance
=cut

sub new {
	my ($class, $dict) = @_;
	my $self = {
		dict => {}
	};

	bless($self, $class);

	if (!defined($dict)) {
		return $self;
	}

	if (ref($dict) eq "HashMap") {
		$dict->forEach(sub {
			my ($key, $val) = @_;
			$self->set($key, $val);
		});
	} elsif (ref($dict) eq "HASH") {
		return HashMap::fromPerlHash(%$dict);
	}

	return $self;
};

=head2 fromPerlHash
	Constructs a HashMap from a given Perl hash.

	Usage:
		my %perlHash = ('a' => 123, ...);
		my $perlHashRef = {};

		my $dict = HashMap::fromPerlHash(%perlHash);
		my $dict_alt = HashMap::fromPerlHash(%$perlHashRef);
		

	Parameters:
		- %perlHash : hash
			The Perl hash that is used to initialize the HashMap instance.

	Returns:
		- The HashMap instance
=cut

sub fromPerlHash {
	my (%perlHash) = @_;
	my $dict = new HashMap();

	for my $key (CORE::keys(%perlHash)) {
		my $val = $perlHash{$key};
		$dict->set($key, $val);
	}

	return $dict;
};

=head2 set
	Adds or updates an entry in the HashMap with a specified key and a value.

	Usage:
		my $dict = new HashMap();
		$dict->set("a", 123);

	Parameters:
		- $key : any
			The key of the element to add to the HashMap.
		- $value : any
			The value of the element to add to the HashMap.
=cut

sub set {
	my ($self, $k, $v) = @_;
	my $dict = $self->{dict};

	$dict->{$k} = $v;
};

=head2 get
	Returns a specified element from the HashMap.

	Usage:
		my $dict = new HashMap();
		$dict->set("a", 123);
		my $val = $dict->get("a");

	Parameters:
		- $key : any
			The key of the element to return from the HashMap.

	Returns:
		- The value associated with the key. Returns undef if the key does not exist in the HashMap.
=cut

sub get {
	my ($self, $k) = @_;
	my $dict = $self->{dict};

	if (!$self->has($k)) {
		return undef;
	}

	return $dict->{$k};
};

=head2 clear
	Removes all elements from the HashMap.

	Usage:
		my $dict = new HashMap();
		...
		$dict->clear();
=cut

sub clear {
	my ($self) = @_;

	my @keys = $self->keys();
	for my $key (@keys) {
		$self->delete($key);
	}
};

=head2 delete
	Removes the specified element from the HashMap by key.

	Usage:
		my $dict = new HashMap();
		...
		$dict->remove("a");

	Parameters:
		- $key : any
			The key of the element to remove from the HashMap.

	Returns:
		- 1 if the element existed prior to removal, 0 otherwise.
=cut

sub delete {
	my ($self, $k) = @_;
	my $dict = $self->{dict};

	my $element_existed = $self->has($k);
	delete $dict->{$k};
	return $element_existed;
};

=head2 size
	Returns the number of elements in the HashMap.

	Usage:
		my $dict = new HashMap();
		my $size = $dict->size();

	Returns:
		- The number of elements in the HashMap.
=cut

sub size {
	my $self = shift;
	my %dict = %{$self->{dict}};

	return scalar(%dict);
};

=head2 has
	Checks to see if an element with a specified key exists in the HashMap.

	Usage:
		my $dict = new HashMap();
		my $contains = $dict->has("a");

	Parameters:
		- $key : any
			The key of the element to test for presence in the HashMap.

	Returns:
		- 1 if an element with the specified key exists, 0 otherwise.
=cut

sub has {
	my ($self, $k) = @_;
	my $dict = $self->{dict};

	return exists($dict->{$k}) ? 1 : 0;
};

=head2 keys
	Gets the keys of the HashMap.

	Usage:
		my $dict = new HashMap();
		my @keys = $dict->keys();

	Returns:
		- A Perl array containing the HashMap's keys.
=cut

sub keys {
	my $self = shift;
	my %dict = %{$self->{dict}};

	return CORE::keys(%dict);
};

=head2 entries
	Gets the (key, value) entries of the HashMap.

	Usage:
		my $dict = new HashMap();
		my @entries = $dict->entries();

	Returns:
		- A Perl array of array references containing the HashMap's (key, value) entries.
=cut

sub entries {
	my $self = shift;
	my %dict = %{$self->{dict}};

	my @arr = ();
	while (my ($k, $v) = each %dict) {
		my @entry = ($k, $v);
		CORE::push(@arr, \@entry);
	}

	return @arr;
};

=head2 values
	Gets the values of the HashMap.

	Usage:
		my $dict = new HashMap();
		my @values = $dict->values();

	Returns:
		- A Perl array containing the HashMap's values.
=cut

sub values {
	my $self = shift;
	my %dict = %{$self->{dict}};

	return CORE::values(%dict);
};

=head2 forEach
    Executes a function over the HashMap's elements.

    Usage:
        my $closure = sub { ... };

        $dict->forEach(sub { ... });
        $dict->forEach($closure);

    Parameters:
        - $func : sub
            Function that will be used on the HashMap's elements.
            Arguments: $key, $value
=cut

sub forEach {
	my ($self, $func) = @_;
	my %dict = %{$self->{dict}};

	while (my ($k, $v) = each %dict) {
		$func->($k, $v);
	}
}

=head2 to_raw_hash_ref

    Gets the raw representation of the HashMap (as a copy ref).

    Usage:
        my $dict = new HashMap();
        ...
        my $raw_dict_ref = $dict->to_raw_hash_ref();

    Returns:
        - The raw representation of the HashMap (as a hash reference).
=cut

sub to_raw_hash_ref {
	my $self = shift;
	my %perlHash = $self->to_raw_hash();
	return \%perlHash;
};

=head2 to_raw_hash

    Gets the raw representation of the HashMap (as a copy).

    Usage:
        my $dict = new HashMap();
        ...
        my %raw_dict = $dict->to_raw_hash();

    Returns:
        - The raw representation of the HashMap.
=cut

sub to_raw_hash {
	my $self = shift;
	my %dict_copy = ();

	$self->forEach(sub {
		my ($key, $value) = @_;
		$dict_copy{$key} = $value;
	});

	return %dict_copy;
};

=head2 to_string

    Gets a string representation of the HashMap.

    Usage:
		my $dict_str = $dict->to_string();

    Parameters:
        - $no_brackets : boolean
            If true, the string will not include brackets.

    Returns:
        - The string representation of the HashMap.
=cut

sub to_string {
	my $self = shift;
	my $no_brackets = shift;

	my @arr = ();
	$self->forEach(sub {
		my ($k, $v) = @_;
		CORE::push(@arr, "($k,$v)");
	});

	if ($no_brackets) {
		return CORE::join(", ", @arr);
	}

	return "{" . join(", ", @arr) . "}";
};

1;
