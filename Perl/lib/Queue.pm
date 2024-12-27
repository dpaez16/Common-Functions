package Queue;

sub new {
	my ($class, $elements) = @_;
	my $self = {
		q => {},
		head => 0,
		tail => 0
	};

	bless($self, $class);

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

sub push {
	my ($self, $elem) = @_;
	my $q = $self->{q};

	$q->{$self->{tail}++} = $elem;
};

sub pop {
	my $self = shift;
	my $q = $self->{q};

	my $elem = $self->front();
	delete $q->{$self->{head}++};
	return $elem;
};

sub front {
	my $self = shift;
	my $q = $self->{q};

	return $q->{$self->{head}};
};

sub size {
	my $self = shift;

	return $self->{tail} - $self->{head};
};

sub empty {
	my $self = shift;

	return $self->size() == 0;
};

sub to_string {
	my $self = shift;
	my $q = $self->{q};
	my @arr = ();
	
	for (my $idx = $self->{head}; $idx < $self->{tail}; $idx++) {
		my $elem = $q->{$idx};
		CORE::push(@arr, $elem);
	}

	return "[" . join(", ", @arr) . "]";
};

1;
