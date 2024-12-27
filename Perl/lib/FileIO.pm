=head1  Description

Static library for file I/O functions.

=cut

package FileIO;

=head2 ReadFile
    Reads a file and returns its lines.

    Usage:
        my @lines = FileIO::ReadFile("/path/to/file");

    Parameters:
        - $filename : string
            The file to be read.

    Returns:
        A Perl array of the file's contents (line-by-line);
=cut

sub ReadFile {
    my ($filename) = @_;

    open(my $fh, '<', $filename) || die "Failed to open $filename: $!";
    my @lines = ();

    while (<$fh>) {
        my $line = $_;
        chomp($line);
        CORE::push(@lines, $line);
    }

    return @lines;
}

=head2 puts
    Prints the string with a trailing newline character.

    Usage:
        FileIO::puts("Hello World!");

    Parameters:
        - $str : string | undef
            The data to print to the console.
=cut

sub puts {
	my $s = shift;

	print("$s\n") if defined($s);
	print("undef\n") if !defined($s);
}

1;