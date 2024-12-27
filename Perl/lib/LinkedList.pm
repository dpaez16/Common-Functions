package LinkedList;

sub new {
    my ($class, $elements) = @_;
    my $self = {
        head => undef,
        tail => undef,
        size => 0,
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
}

sub push {
    my ($self, $elem) = @_;

    my $new_node = {
        val => $elem,
        next_node => undef,
        prev_node => undef,
    };

    if ($self->size() == 0) {
        $self->{head} = $new_node;
        $self->{tail} = $new_node;
    } else {
        $self->{tail}->{next_node} = $new_node;
        $new_node->{prev_node} = $self->{tail};
        $self->{tail} = $new_node;
    }

    $self->{size}++;
}

sub head {
    my ($self) = @_;
    return $self->{head};
}

sub tail {
    my ($self) = @_;
    return $self->{tail};
}

sub size {
    my ($self) = @_;
    return $self->{size};
}

sub to_string {
    my ($self) = @_;

    my @arr = ();
    my $ptr = $self->head();

    while ($ptr) {
        CORE::push(@arr, $ptr->{val});
        $ptr = $ptr->{next_node};
    }

    return "[ " . CORE::join(", ", @arr) . " ]";
}

1;