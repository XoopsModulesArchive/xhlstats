<?php
	/*
	 * HLstats - Real-time player and clan rankings and statistics for Half-Life
	 * http://sourceforge.net/projects/hlstats/
	 *
	 * Copyright (C) 2001  Simon Garner
	 *
	 * This program is free software; you can redistribute it and/or
	 * modify it under the terms of the GNU General Public License
	 * as published by the Free Software Foundation; either version 2
	 * of the License, or (at your option) any later version.
	 *
	 * This program is distributed in the hope that it will be useful,
	 * but WITHOUT ANY WARRANTY; without even the implied warranty of
	 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 * GNU General Public License for more details.
	 *
	 * You should have received a copy of the GNU General Public License
	 * along with this program; if not, write to the Free Software
	 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
	 */
   
   // Half-Life Query Functions
   
   // These functions with the use of Detritus' binary functions allow HL servers to be queried
   // and data to be returned in a friendly readable format
function GetServerData($command, $ip, $port)
{
	$data = '';
	if (!$server_connection = fsockopen('udp://'.$ip, $port))
	{
		# Couldn't establish a connection
		return false;
	}
	stream_set_blocking($server_connection, 1);
	# Time out after 5 seconds
	stream_set_timeout($server_connection, 5);
	fwrite($server_connection, $command, strlen($command));	

	# This must time out first to make sure the right arguement is returned
	# The above timeout is just to make sure it doesn't go over 4 seconds
	$timeout = microtime();
	$timeout += 4;
	$i = 0;
	$packets = 1;
	do
	{
		do
		{
			$data .= fgetc($server_connection);
			if (microtime() > $timeout)
			{
				# Connection timed out
				fclose($server_connection);
				return false;
			}
			$data_status = stream_get_meta_data($server_connection);
		}
		while ($data_status['unread_bytes']);
		
		if(GetInt8($data, $tmp = 0) == 254)
		{
			# We have a split packet!
			$tmp = 0;
			# Get rid of the crap at the start
			GetInt32($data, $tmp);
			GetInt32($data, $tmp);
			$split_packet = str_pad(decbin(GetInt8($data, $tmp)), 8, 0, STR_PAD_LEFT);
			$packet_num = bindec(substr($split_packet, 0, 4));
			$packets = bindec(substr($split_packet, 4));

			$data = substr($data, $tmp);
			$packet_data[$packet_num] = $data;
			$data = '';
		}
		$i++;
	}
	while($i < $packets);
	
	if($packets > 1)
	{
		ksort($packet_data);
		$data = implode('', $packet_data);
	}
	fclose($server_connection);

	if(strlen($data) == 0)
	{
		# We got no data?! Something must have gone wrong
		return false;
	}
	
	return $data;
}

function HalfLife_Rules($serveraddress, $portnumber)
{
	$cmd = "\xFF\xFF\xFF\xFFrules\x00";
	if(!$rule_data = GetServerData($cmd, $serveraddress, $portnumber))
	{
		return false;
	}
	$datastart = 0;
	GetInt32($rule_data, $datastart);
	GetInt8($rule_data, $datastart);
	$rules = GetInt16($rule_data, $datastart);
	$serverrules = '';
	if ($rules != 0)
	{
		for ($i = 1; $i <= $rules; $i++)
		{
			$rulename = GetString($rule_data, $datastart);
			$rulevalue = GetString($rule_data, $datastart);
			$serverrules[$rulename] = $rulevalue;
		}
		return $serverrules;
	}
	else
		return false;
}	

function HalfLife_Players($serveraddress, $portnumber)
{
	$cmd = "\xFF\xFF\xFF\xFFplayers\x00";

	if(!$serverdata = GetServerData($cmd, $serveraddress, $portnumber))
	{
		return false;
	}

	$datastart = 0;
	GetInt32($serverdata, $datastart);
	GetInt8($serverdata, $datastart);
	$players = GetInt8($serverdata, $datastart);
	if($players == 0)
		return false;

	for ($i = 0; $i < $players; $i++)
	{
		$index = GetInt8($serverdata, $datastart);
		$name = htmlspecialchars(GetString($serverdata, $datastart), ENT_QUOTES | ENT_HTML5);
		$frags = GetInt32($serverdata, $datastart);
		$time = GetFloat32($serverdata, $datastart);

		$serverplayers[$i] = ['index' => $index, 'name' => $name, 'frags' => $frags, 'time' => $time];
	}
		return $serverplayers;
}
	
function HalfLife_Details($serveraddress, $portnumber)
{
	$cmd = "\xFF\xFF\xFF\xFFdetails\x00";

	if(!$serverdata = GetServerData($cmd, $serveraddress, $portnumber))
	{
		return false;
	}

	$datastart = 0;
	GetInt32($serverdata, $datastart);
	GetInt8($serverdata, $datastart);
	$serverdetails['address'] = GetString($serverdata, $datastart);
	$serverdetails['hostname'] = GetString($serverdata, $datastart);
	$serverdetails['map'] = GetString($serverdata, $datastart);
	$serverdetails['gamedir'] = GetString($serverdata, $datastart);
	$serverdetails['description'] = GetString($serverdata, $datastart);
	$serverdetails['players'] = GetInt8($serverdata, $datastart);
	$serverdetails['max'] = GetInt8($serverdata, $datastart);
	$serverdetails['protocol'] = GetInt8($serverdata, $datastart);
	$serverdetails['type'] = chr(GetInt8($serverdata, $datastart));
	$serverdetails['os'] = chr(GetInt8($serverdata, $datastart));
	$serverdetails['password'] = GetInt8($serverdata, $datastart);
	$serverdetails['ismod'] = GetInt8($serverdata, $datastart);
	if($serverdetails['ismod'])
	{
		$serverdetails['mod_website'] = GetString($serverdata, $datastart);
		$serverdetails['mod_download'] = GetString($serverdata, $datastart);
		$serverdetails['hl_version'] = GetString($serverdata, $datastart);
		$serverdetails['mod_version'] = GetInt32($serverdata, $datastart);
		$serverdetails['mod_size'] = GetInt32($serverdata, $datastart);
		$serverdetails['mod_type'] = GetInt8($serverdata, $datastart);
		$serverdetails['mod_dll'] = GetInt8($serverdata, $datastart);		
	}
	$serverdetails['secure'] = GetInt8($serverdata, $datastart);
	$serverdetails['botcount'] = GetInt8($serverdata, $datastart);

	if($serverdetails['type'] == 'd')
		$serverdetails['type'] = 'Dedicated';
	elseif($serverdetails['type'] == 'l')
		$serverdetails['type'] = 'Listen';
	elseif($serverdetails['type'] == 'p')
		$serverdetails['type'] = 'Proxy';

	if($serverdetails['os'] == 'w')
		$serverdetails['os'] = 'Windows';
	elseif($serverdetails['os'] == 'l')
		$serverdetails['os'] = 'Linux';

	if($serverdetails['password'] == 0)
		$serverdetails['password'] = 'No';
	elseif($serverdetails['password'] == 1)
		$serverdetails['password'] = 'Yes';

	if($serverdetails['secure'] == 0)
		$serverdetails['secure'] = 'No';
	elseif($serverdetails['secure'] == 1)
		$serverdetails['secure'] = 'Yes';

	return $serverdetails;
}

function HalfLife_GetChallenge($serveraddress, $portnumber)
{
	$cmd = "\xFF\xFF\xFF\xFFchallenge rcon\n";

	if(!$serverdata = GetServerData($cmd, $serveraddress, $portnumber))
	{
		return false;
	}
	$serverdata = substr($serverdata, 19);
	$serverdata = trim($serverdata);
	
	return $serverdata;
}

function HalfLife_Rcon($serveraddress, $portnumber, $rcon, $command, &$challenge, $challenged = 0)
{
	# No point challenging more than we have too.
	if(strlen($challenge) < 5)
	{
		$challenge = HalfLife_GetChallenge($serveraddress, $portnumber);
		$challenged++;
	}

	$cmd = "ÿÿÿÿrcon $challenge \"$rcon\" $command";
	if(!$serverdata = GetServerData($cmd, $serveraddress, $portnumber))
	{
		return false;
	}
	
	$serverdata = substr($serverdata, 5);
	$serverdata = trim($serverdata);
	if(ereg('^Bad challenge.', $serverdata))
	{
		if($challenged > 1)
		{
			$challenge = '';
			$serverdata = HalfLife_Rcon($serveraddress, $portnumber, $rcon, $command, $challenge, $challenged);
		}
	}
	elseif(ereg('^Bad rcon_password.', $serverdata))
	{
		return false;
	}
	return $serverdata;
}
	
function Formated_Times($temp)
{
	$time = sprintf('%s', str_pad(floor($temp/3600), 2, 0, STR_PAD_LEFT));
	$temp %= 3600;
	$time = sprintf('%s:%s', $time, str_pad(floor($temp/60), 2, 0, STR_PAD_LEFT));
	$temp %= 60;
	$time = sprintf('%s:%s', $time, str_pad(floor($temp), 2, 0, STR_PAD_LEFT));
	return $time;
}

