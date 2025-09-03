#!perl

#date Sat Aug 20 16:51:02 2011
#base hex  timestamps absolute
#internal events logged
#   0.000000 Start of measurement
#Begin Triggerblock
#;  time    can ident           attr dlc data...Sat Aug 20 16:51:02 2011
#   2.395000    log trigger event
#   2.395950 1  28a             Rx   d 8 55 49 05 00 00 00 00 00
#   2.399400 1  30a             Rx   d 8 49 50 00 40 00 00 00 00
#   2.400000 1  800002x         Rx   d 8 03 00 00 00 00 03 E8 00
#   2.400750 1  800001x         Rx   d 8 03 00 00 00 00 03 E8 00

use strict;
use Math::BigInt;

my %inf;
my @lines;
my $txt = shift( @ARGV);
my $dbc = shift( @ARGV);
my $t0 = -1;

my %dbc_inf = dbcinf();

get_lines();
proc_lines( \@lines);
undef @lines;
txtproc();

debug();

sub get_lines
{
    open( FH, "< $txt") || die( "Cant open $txt.\n");
    print "Slurping file...\n";
    while( <FH>)
    {
        push( @lines, $_)
		 if( m/\s*(\d+.\d+)\s+(\d)\s+([\d\w]+)\s+/)
#         if( m/([[:alnum:]]*[.]*[[:alnum:]]+),([[:alnum:]]{1}),([[:alnum:]]+),([[:alnum:]]{1}),([[:alnum:]]{1}),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]+),/)
    }
    close FH;
}

sub proc_lines
{
    my $r_lines = shift( @_);
    my @lines = @$r_lines;
    print "Got " . $#lines . " messages...\n";
    print "Processing text...\n";
    #foreach (@lines)
    while ($#lines >= 0)
    {
        #print $#lines . "\n";
        my $line = shift( @lines);
		if( $line =~ m/\s*(\d+.\d+)\s+(\d)\s+([\d\w]+)[x]*\s+.*Rx\s+d\s+(\d)\s+(.*)$/)
        {
			my $id_ = $3;
#			$id_ =~ s/x//;
			my $id = sprintf( "%03X", hex($id_));
			my $ch = sprintf( "%01X", $2);
			my $dlc = $4;
			my $data = $5;
			my @data = split( /\s+/, $data);
			#print "@data\n";
#   2.395950 1  28a             Rx   d 8 55 49 05 00 00 00 00 00
#   2.399400 1  30a             Rx   d 8 49 50 00 40 00 00 00 00
#   2.400000 1  800002x         Rx   d 8 03 00 00 00 00 03 E8 00
#   2.400750 1  800001x         Rx   d 8 03 00 00 00 00 03 E8 00
#        if( $line =~ m/([[:alnum:]]*[.]*[[:alnum:]]+),([[:alnum:]]{1}),([[:alnum:]]+),([[:alnum:]]{1}),([[:alnum:]]{1}),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]+),/)
		#Time,Channel,id,Flags,DLC,Data0,Data1,Data2,Data3,Data4,Data5,Data6,Data7,Counter,AbsTime
			my $tstamp = $1;
#			my $data = $4;
#			my @data = ($6,$7,$8,$9,$10,$11,$12,$13);
            push( @{$inf{$ch}{$id}{ts}}, $tstamp);
#			push( @{$inf{$ch}{$id}{data}}, $data);
#			$inf{$id}{ch} = $ch;
			foreach (@data)
			{
				$_ = hex( $_); #sprintf( "%02X", hex( $_));
			}
            push( @{$inf{$ch}{$id}{data}}, join( ",", @data));
            if( $t0 < 0)
            {
                $t0 = $tstamp;
            }
        }
    }
}

sub txtproc
{
	foreach my $ch (keys %inf)
	{
		my %ch_inf = %{$inf{$ch}};
		foreach my $key (keys %ch_inf)
		{
		print "$ch : $key\n";
			my $str = '';
#			my $ch = $inf{$key}{ch};
			my $csv = "x" . $key . "_ch" . $ch . ".csv";
			my @ts = @{$inf{$ch}{$key}{ts}};
			my @data = @{$inf{$ch}{$key}{data}};
#			my @data = @{$inf{$ch}{$key}{data}};
			foreach (@ts)
			{
				my $t = $_ - $t0; # * $dt - $t0;
#				my $data = shift( @data);
#				$str .= "$t,";
#				$str .= "\n";
				
				my @data_ = split( /,/, shift( @data));
				foreach (@data_)
				{
					$_ = sprintf( '%02X', $_);
				}
				my $data_str = hex2eng( $key, @data_);
				
				$str .= "$t,$data_str\n";
			}
			undef $inf{$ch}{$key};
			#delete( $inf{$key});

			open( FH, "> $csv") || die( "Cant create $csv.\n");
			print FH csvhdr( $key);
			print FH $str;
			close FH;
			undef $str;
		}
	}
}

sub hex2eng
{
    my $id = shift( @_);
    my @data = @_;
    my @eng;
    #print "$id : @data\n";
    foreach my $sig ( keys %{$dbc_inf{"x$id"}})
    {
        #print "$sig : " . $dbc_inf{"x$id"}{$sig}{start_bit} . "\n";
        my $start_bit = $dbc_inf{"x$id"}{$sig}{start_bit};
        my $length = $dbc_inf{"x$id"}{$sig}{length};
        my $endian = $dbc_inf{"x$id"}{$sig}{endian};
        my $sign = $dbc_inf{"x$id"}{$sig}{sign};
        my $m = $dbc_inf{"x$id"}{$sig}{m};
        my $b = $dbc_inf{"x$id"}{$sig}{b};
        my $x = get_bits( $start_bit, $length, $endian, $sign, \@data);
        if( $sign eq "-" && ($x & (1<<$length-1)))
        {
            #print "$hex_str : $x \n";
            $x = ~$x;
            $x = $x & ((1<<$length)-1);
            $x = -$x;
            #print "$hex_str : $x \n";
        }
        my $units = $dbc_inf{"x$id"}{$sig}{units};
        if( $units =~ m/single/)
        {
            #print "found a single... ";
            #print "$x ";
            $x = sgl2dec( $x);
            #print "$x\n";
        }
        my $y = $m * $x + $b;
        #print "$sig : $y\n";
        push( @eng, $y);
    }
    return join( ',', @eng);
}

sub get_bits
{
    my $start_bit = shift( @_);
    my $length = shift( @_);
    my $endian = shift( @_);
    my $sign = shift( @_);
    my $r_data = shift( @_);
    my $hex_str;
    my $x;
    if( $endian == 1) # le
    {
        $hex_str = join( '', reverse(@$r_data));
    }
    else
    {
        $hex_str = join( '', @$r_data);
    }
    # HACK
    if( $start_bit < 32)
    {
        $hex_str = substr( $hex_str, 8, 8);
        $x = hex( $hex_str) >> $start_bit;
    }
    else
    {
        $hex_str = substr( $hex_str, 0, 8);
        $x = hex( $hex_str) >> ($start_bit - 32);
    }
    $x <<= (32-$length);
    $x >>= (32-$length);
    #print "$hex_str : $x\n";
    return $x;
}

sub csvhdr
{
    my $id = shift( @_);
    my $str = 't';
    foreach my $sig ( keys %{$dbc_inf{"x$id"}})
    {
        $str .= "," . $sig;
    }
    $str .= "\n";
    return $str;
}

sub dbcinf
{
    open( FH, "< $dbc") || die( "Cant open $dbc.\n");
    my %inf;
    while( <FH>)
    {
        my $line = $_;
        if( m/BO_ (\d+) (.*): (\d+) (\w+)/)
        {
            my $id = $2;
            #my $n_sig = $3;
            my $n;
            for ($n=0; 1; $n++)
            {
                $_ = <FH>;
                last if !m/SG_/;
                #print "$_";
                if( m/SG_\s*(\w+):\s*(\d+)\|(\d+)\@(\d+)([+-])\s*\((.*),(.*)\)\s*\[(.*)\|(.*)\]\s*\"(.*)\"/)
                {
                    my $sig = $1;
                    my $r_sig_info =
                    {
                        sig => $sig,
                        start_bit => $2,
                        length => $3,
                        endian => $4,
                        sign => $5,
                        m => $6,
                        b => $7,
                        min => $8,
                        max => $9,
                        units => $10,
                    };
                    $inf{$id}{$sig} = $r_sig_info;
                }
                else { print $_;}
            }
            #$inf{$id}{n} = $n;
        }
    }
    close FH;

    return %inf;
}

sub debug
{
use Data::Dumper;
	open( FH, "> dbg.txt");
	print FH Dumper( %inf);
    print FH Dumper( %dbc_inf);
	close FH;
}
