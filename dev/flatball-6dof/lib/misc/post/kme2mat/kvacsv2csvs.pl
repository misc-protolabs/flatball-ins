#!perl
use strict;
use Math::BigInt;
use POSIX;

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

#debug();

sub get_lines
{
    open( FH, "< $txt") || die( "Cant open $txt.\n");
    print "Slurping file...\n";
    while( <FH>)
    {
		#0.18916,1,418381679,4,8,137,152,0,0,5,0,255,255,1,2011-07-21 11:07:12
        push( @lines, $_)
         if( m/([[:alnum:]]*[.]*[[:alnum:]]+),([[:alnum:]]{1}),([[:alnum:]]+),([[:alnum:]]{1}),([[:alnum:]]{1}),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]+),/)
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
        if( $line =~ m/([[:alnum:]]*[.]*[[:alnum:]]+),([[:alnum:]]{1}),([[:alnum:]]+),([[:alnum:]]{1}),([[:alnum:]]{1}),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]*),([[:alnum:]]+),/)
        {
		#Time,Channel,id,Flags,DLC,Data0,Data1,Data2,Data3,Data4,Data5,Data6,Data7,Counter,AbsTime
			my $flgs = hex($4);
			my $id;
			if( $flgs & 0x4)
			{
				$id = sprintf( "%08X", $3);
			}
			else
			{
				$id = sprintf( "%03X", $3);
			}
			my $ch = sprintf( "%01X", $2);
			my $tstamp = $1;
			my @data = ($6,$7,$8,$9,$10,$11,$12,$13);
            push( @{$inf{$ch}{$id}{ts}}, $tstamp);
#			$inf{$id}{ch} = $ch;
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
		print "$key\n";
			my $str = '';
#			my $ch = $inf{$key}{ch};
			my $csv = "x" . $key . "_ch" . $ch . ".csv";
			my @ts = @{$inf{$ch}{$key}{ts}};
			my @data = @{$inf{$ch}{$key}{data}};
			foreach (@ts)
			{
				my $t = $_ - $t0; # * $dt - $t0;
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

sub hex2hex
{
    my @data = @_;
    foreach( @data)
    {
        $_ = hex( $_);
    }
    return join( ',', @data);
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
		if( $sig eq 'gear_position')
		{
			print "$start_bit : $length : $endian : $sign : $x\n";
		}
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

sub sgl2dec
{
    my $sgl = shift( @_);
    my $sgn = (int($sgl) & 0x80000000) ? -1 : 1;
    my $exp = ((int($sgl) & 0x7f800000) >> 23) - 127;
    my $sig = (int($sgl) & 0x007fffff) * 2**-23 + 1;
    my $dec = $sgn * $sig * 2**$exp;
    #print "$sgl : $sgn : $exp : $sig : $dec\n";
    return $dec;
}

sub dec2sgl
{
    # -1^s * sig * base^exp
    my $dec = shift( @_);
    my $hex = reverse( unpack( 'h*', pack( 'f', $dec)));
    #my $sgn = 0;
    #my $sgn = ($hex & 0x80000000) ? -1 : 1;
    #my $exp = (($hex & 0x7f800000) >> 23) - 127;
    #my $sig = ($hex & 0x007fffff) * 2^-23 + 1;
    #my $sgl = $sgn * $sig * 2^$exp;
    #print "$dec : $hex : $sgn : $exp : $sig : $sgl\n";
    #return $sgl;
    return hex($hex);
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

	my $sb = cnvrt_start_bit_dbc_( $start_bit, $endian, $length);

	my $data = '';
	if( $endian == 1) # le
	{
		$data = join( '', ( @$r_data));
	}
	else
	{
		$data = join( '', @$r_data);
	}

	my $start_byte = 8 - POSIX::floor( (64 - $sb)/8);
	my $end_byte = $start_byte;
	if( $length > 8)
	{
		$end_byte = $start_byte + POSIX::floor( $length/8) - 1;
	}

	my $hex_str = '';
	if( $endian == 1) # le
	{
		if( $start_byte == $end_byte)
		{
			$hex_str = ( substr( $data, $start_byte*2, 2));
		}
		else
		{
			my $tmp_hex_str = ( substr( $data, $start_byte*2, ($end_byte - $start_byte + 1)*2));
			while( length( $tmp_hex_str) > 0)
			{
				$hex_str .= substr( $tmp_hex_str, -2, 2);
				$tmp_hex_str = substr( $tmp_hex_str, 0, -2);
			}
		}
	}
	else
	{
		$hex_str = ( substr( $data, $start_byte*2, ($end_byte - $start_byte + 1)*2));
	}

	my $mask = 2**$length - 1;
	$x = hex( $hex_str) & $mask;
	#$x = (hex( $hex_str) >> (POSIX::floor($sb/8))) & $mask;

	#print "$sb : $start_byte : $end_byte : $length :: $hex_str :: $x\n";

	#print "@data\n$data\n$start_byte : $end_byte\n$hex_str : $mask\n$x\n";
    return $x;
}

sub cnvrt_start_bit
{
    my $start_bit = shift( @_);
    my $endian = shift( @_);
    my $sb;

    if( !$endian)
    {
        my $tmp1 = POSIX::floor( ($start_bit / 8)+1)*8;
        my( $tmp2_f, $tmp2_i) = POSIX::modf( $start_bit / 8);
        $sb = 64 - $tmp1 + $tmp2_f*8;
    }
    else
    {
        $sb = $start_bit;
    }
    return $sb;
}

sub cnvrt_start_bit_dbc_
{
    my $start_bit = shift( @_);
    my $endian = shift( @_);
    my $width = shift( @_);
    my $sb;
    my @bits = (
			7,6,5,4,3,2,1,0,
			15,14,13,12,11,10,9,8,
			23,22,21,20,19,18,17,16,
			31,30,29,28,27,26,25,24,
			39,38,37,36,35,34,33,32,
			47,46,45,44,43,42,41,40,
			55,54,53,52,51,50,49,48
            );
    if( $endian)
    {
		$sb = $start_bit;
    }
    else
    {
        $sb = $bits[ $start_bit];
    }
    return $sb;
}

sub cnvrt_start_bit_dbc
{
    my $start_bit = shift( @_);
    my $endian = shift( @_);
    my $width = shift( @_);
    my $sb;
    my @bits = (
            56,57,58,59,60,61,62,63,
            48,49,50,51,52,53,54,55,
            40,41,42,43,44,45,46,47,
            32,33,34,35,36,37,38,39,
            24,25,26,27,28,29,30,31,
            16,17,18,19,20,21,22,23,
            8,9,10,11,12,13,14,15,
            0,1,2,3,4,5,6,7
            );
    if( $endian)
    {
        $sb = $bits[ $start_bit + $width - 1];
    }
    else
    {
        $sb = $bits[ $start_bit];
    }
    return $sb;
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
