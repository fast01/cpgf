use strict;
use warnings;

my @commonReplaceToGlobal = (
	qr'#define\s+CLASS\s+.*' => '#define CLASS void',
	qr'.*class\s+CLASS\s+.*' => '',
	qr'.*class\s+CLASS\s*$' => '',
	qr'^\s*public:.*' => '',
	qr'^\s*protected:.*' => '',
	qr'^\s*private:.*' => '',
	qr'\bvirtual\b' => '',
	qr'pobj\-\>' => '',
	qr'GMETA_DEFINE_CLASS\(.*\)' => 'GMETA_DEFINE_GLOBAL()',
	qr'&CLASS::' => '&',
	qr'^\s*CLASS\s+instance.*' => '',
	qr'&instance' => 'NULL',
	qr'findMetaClass\s*\(.*?\)' => 'getGlobalMetaClass()',
	qr'findClassByName\s*\(.*?\)' => 'getGlobalMetaClass()',
	qr'const\s*\{\s*$' => '{',
	qr'const\s*$' => '',
);

my @replaceItems = (
	{
		input => 'test_reflection_field.cpp',
		output => 'autogen_test_reflection_global_field.cpp',
		
		replace => [
			@commonReplaceToGlobal,

			qr'GMETA_FIELD' => 'GMETA_QUALIFIED_FIELD',
			qr'\bTest_Field\b' => 'Test_GlobalField',
		],
	},

	{
		input => 'test_reflection_property.cpp',
		output => 'autogen_test_reflection_global_property.cpp',
		
		replace => [
			@commonReplaceToGlobal,

			qr'GMETA_PROPERTY' => 'GMETA_QUALIFIED_PROPERTY',
			qr'\bTest_Property\b' => 'Test_GlobalProperty',
		],
	},

	{
		input => 'test_reflection_method.cpp',
		output => 'autogen_test_reflection_global_method.cpp',
		
		replace => [
			@commonReplaceToGlobal,

			qr'GMETA_METHOD' => 'GMETA_QUALIFIED_METHOD',
			qr'\bTest_Method\b' => 'Test_GlobalMethod',
		],
	},
);

&doMain;

sub doMain
{
	foreach(@replaceItems) {
		&processFile($_);
	}
}

sub processFile
{
	my ($item) = @_;
	
	if(not open FH, '<' . $item->{input}) {
		print "Can't open file $item->{input} to read.\n";
		
		return;
	}
	
	my @lines = <FH>;
	
	close FH;
	
	my $newLines = &processReplace($item->{replace}, \@lines);

	if(not open FH, '>' . $item->{output}) {
		print "Can't open file $item->{output} to write.\n";
		
		return;
	}

	print FH <<EOM;
// Auto generated test code
// Don't modify manually.

EOM
	print FH @$newLines;
	close FH;
}

sub processReplace
{
	my ($replaces, $lines) = @_;
	my $newLines = [];
	
	my $replaceCount = $#{@$replaces} + 1;
	
	foreach(@$lines) {
		my $line = $_;
		$line =~ s/\n$//;
		
		for(my $i = 0; $i < $replaceCount; $i += 2) {
			my $from = $replaces->[$i];
			my $to = $replaces->[$i + 1];
			
			$line =~ s/$from/$to/g;
		}
		push @$newLines, $line . "\n";
	}
	
	return $newLines;
}