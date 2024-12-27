package String;

=head1  Description

Static library for string functions.

=cut

=head2 atoi
    Converts a string into an integer value.

    Usage:
        my $num = String::atoi($str);

    Parameters:
        - $str : string
            The string value to convert into an integer value.

    Returns:
        The converted integer value. Returns 0 if unable to convert the string.
=cut

sub atoi {
    my ($str) = @_;
    if (!defined($str)) {
        return 0;
    }

    if ($str =~ /^[-+]?\d+$/) {
        return $str + 0;
    } else {
        return 0;
    }
}

=head2 to_char_array
    Converts a string into a C-style array of characters.

    Usage:
        my @chars = String::to_char_array("abcd");

    Parameters:
        - $str : string
            The string to convert to the array of characters.

    Returns:
        A Perl array of the string's characters.
=cut

sub to_char_array {
    my ($str) = @_;
    if (!defined($str)) {
        return ();
    }

    return split //, $str;
}

=head2 trim
    Trims leading and trailing whitespace off of a string.

    Usage:
        my $trimmed = String::trim(" 123 ");

    Paramaters:
        $str : string
            The string to trim the whitespace from.

    Returns:
        The trimmed string.
=cut

sub trim {
    my ($str) = @_;
    $str =~ s/^\s+|\s+$//g;
    return $str;
}

1;