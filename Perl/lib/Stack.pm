package Stack;

sub new {
	my ($class, $elements) = @_;
	my $self = {
		stk => []
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

sub push {
	my ($self, $elem) = @_;
	my $stk = $self->{stk};

	CORE::push(@$stk, $elem);
};

sub pop {
	my $self = shift;
	my $stk = $self->{stk};

	return CORE::pop(@$stk);
};

sub top {
	my $self = shift;
	my $stk = $self->{stk};

	return $stk->[-1];
};

sub size {
	my $self = shift;
	my $stk = $self->{stk};

	return CORE::scalar(@$stk);
};

sub empty {
	my $self = shift;

	return ($self->size() == 0) ? 1 : 0;
};

sub to_string {
	my $self = shift;
	my $stk = $self->{stk};

	return "[" . join(", ", @$stk) . "]";
};

1;
