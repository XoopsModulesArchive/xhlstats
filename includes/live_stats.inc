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

   // Live Stats

   // The binary functions need to be included
   // Along with the HL Query functions
require __DIR__ . '/binary_funcs.inc';
require __DIR__ . '/hlquery_funcs.inc';

$sql = $xoopsDB->query(
    '
		SELECT
			s.serverId,
			s.name,
			s.address,
			s.port,
			s.publicaddress,
			s.game,
			s.rcon_password,
			g.name gamename
		FROM
			' . $xoopsDB->prefix('hlstats_Servers') . ' s
		LEFT JOIN
			' . $xoopsDB->prefix('hlstats_Games') . " g
		ON
			s.game=g.code
		WHERE
			serverId='$_GET[server]'
			");
if ($xoopsDB->getRowsNum($sql) != 1)
	error('Invalid or no server specified.');
else
	$server = $xoopsDB->fetchArray($sql);

pageHeader(
    ['Server Statistics', $server['name']],
    [
        $server['gamename'] => $g_options['scripturl'] . '?game=' . $server['game'],
        'Server Statistics' => $g_options['scripturl'] . '?game=' . $server['game'],
		$server['name']     => ''
    ]
);

if($server['publicaddress'])
{
	# Port maybe different
	$temp = explode(':', $server['publicaddress']);
	$server_ip = $server['address'];
	$server_port = $temp['1'];
}
else
{
	$server_ip = $server['address'];
	$server_port = $server['port'];
}
	
$server_rcon = $server['rcon_password'];
$server_hltv = [];
$server_players = [];

	
if(!$server_details = HalfLife_Details($server_ip, $server_port))
{
	error("The details for this server couldn't be retrieved, this maybe because the server is currently unavailable.");
}
$server_rules = HalfLife_Rules($server_ip, $server_port);

# If we have an rcon, might as well test it and get some 'extra features' :)
$rcon_worked = 0;
if($server_rcon)
{
	$server_status = HalfLife_Rcon($server_ip, $server_port, $server_rcon, 'status', $server_challenge);

	if($server_status)
	{
		$temp = explode("\n", $server_status);

		$i = count($temp);
		unset($temp[0]); # hostname
		unset($temp[1]); # version
		unset($temp[2]); # tcp/ip
		unset($temp[3]); # map
		unset($temp[4]); # players
		unset($temp[5]); # *blank*
		unset($temp[6]); # header
		if($temp[7] == '0 Users')
		{
			# Server is empty
		}
		else
		{
			unset($temp[$i-1]);
			# We will need multiple counters
			$i = 0;
			$player_i = 0;
			$hltv_i = 0;
			foreach ($temp as $players)
			{
				# Get the name before reformating the string
				eregi('"(.*)"', $players, $tmp);
				$name = $tmp['1'];
				
				# For those people who have more than 1 space in there name
				$name = str_replace(' ', '&nbsp;', $name);
				
				# de-spacalise the string
				$players = preg_replace('/( +)/', ' ', $players);
				
				# Get the de-spacelised name from the de-spacalised string
				eregi('"(.*)"', $players, $tmp);
				$name2 = $tmp['1'];
				
				# get rid of the hashes
				$players = str_replace('# ', '', $players);
				$players = str_replace('#', '', $players);
				
				
				$player = explode(' ', $players);
				$count = substr_count($name2, ' ')+2;

				if($player[$count+1] == 'HLTV')
				{
					# It is possible for multiple HLTVs on 1 server so lets build it like the players.
					$server_hltv[$hltv_i] = [
                        'id'       => $player[0],
                        'name'     => $name,
                        'userid'   => $player[$count++],
                        'uid'      => $player[$count++],
                        'viewers'  => substr($player[$count], 5, 1),
                        'capacity' => substr($player[$count++], 7),
                        'delay'    => substr($player[$count++], 6),
                        'time'     => $player[$count++],
                        'ip'       => $player[$count++]
                    ];
					$hltv_i++;
				}
				else
				{
					$server_players[$player_i] = [
                        'id'     => $player[0],
                        'name'   => $name,
                        'userid' => $player[$count++],
                        'uid'    => $player[$count++],
                        'frags'  => $player[$count++],
                        'time'   => $player[$count++],
                        'ping'   => $player[$count++],
                        'loss'   => $player[$count++],
                        'ip'     => $player[$count++]
                    ];
					$player_i++;
				}
				$i++;
			}
			$rcon_worked = 1;
		}			
	}
}

$server_details['hltvcount'] = count($server_hltv);

if(!$rcon_worked)
	$server_players = HalfLife_Players($server_ip, $server_port);

$server_details['players_real'] = $server_details['players'];
$server_details['players_real'] -= $server_details['botcount'];
$server_details['players_real'] -= $server_details['hltvcount'];

$server_details['players_connecting'] = $server_details['players'];
$server_details['players_connecting'] -= count($server_players);
$server_details['players_connecting'] -= $server_details['hltvcount'];
?>
<table width="90%" align="center" border="0" cellspacing="0" cellpadding="0">
	<tr>
		<td>
			<table width="100%" border="0" cellspacing="1" cellpadding="2">
				<tr>
					<td style="padding: 0px;" width="208" height="163"><img src="<?php
$image = getImage('/maps/'.strtolower($server_details['gamedir']).'/'.strtolower($server_details['map']));
if($image)
{
	echo $image['url'];
}
else
{
	echo $g_options['imgdir'].'/noimage.jpg';
}
?>" width="218" height="164" alt="<?php echo $server_details['map']; ?>"></td>

					<td rowspan="2" valign="top" align="center" class="fontNormal">
						<b><?php echo $server_details['hostname'];?></b><br>
						<a href="<?php echo $g_options['scripturl']; ?>?game=<?php echo $server_details['gamedir'];?>"><?php echo $server_details['description'];?></a><br><br>
						
<?php
if($rcon_worked)
{
	if(count($server_players) > 0)
	{
?>
						<table border="0" width="99%" cellpadding="0">
							<tr>
								<th width="16"></th>
								<th align="left">Name</th>
								<th align="left" width="125">Unique ID</th>
								<th align="right" width="35">Frags</th>
								<th align="right" width="30">Ping</th>
								<th align="right" width="30">Loss</th>
								<th align="right" width="75">Connected</th>
							</tr>
<?php
		$totalfrags = 0;
		$totalping = 0;
		$nonbots = 0;
		foreach($server_players as $player)
		{
?>

							<tr>
<?php
			# figure out if the player is a bot
			$is_bot = 0;
			if($player['uid'] == 'UNKNOWN')
				$is_bot = 1;
			# HLTV is not really a bot we will have to treat it like one.
			if($player['uid'] == 'HLTV')
				$is_bot = 1;
			elseif($player['uid'] == '0')
				$is_bot = 1;
			elseif($player['ping'] == '0')
				$is_bot = 1;
			if($is_bot == 1)
			{
?>

								<td><img src="<?php echo $g_options['imgdir']; ?>/server.gif" alt="Bot"></td>
<?php
				$searchfor = 'BOT:'.md5($player['name'].$server_ip.':'.$server_port);
			}
			else
			{
				# I'm a real boy
				$totalping += $player['ping'];
				$nonbots++;
?>

								<td><img src="<?php echo $g_options['imgdir']; ?>/player.gif" alt="Player"></td>
<?php
				$searchfor = $player['uid'];
			}
?>

								<td align="left" nowrap><a href="<?php echo $g_options['scripturl']; ?>?mode=search&amp;q=<?php echo $searchfor; ?>&amp;st=uniqueid&amp;game=<?php echo $server_details['gamedir']; ?>"><?php echo $player['name']; ?></a></td>
								<td align="left"><?php echo $player['uid']; ?></td>
								<td align="right"><?php echo $player['frags']; ?></td>
								<td align="right"><?php echo $player['ping']; ?></td>
								<td align="right"><?php echo $player['loss']; ?>%</td>
								<td align="right"><?php echo $player['time']; ?></td>
							</tr>
<?php
			$totalfrags += $player['frags'];
		}
?>
							<tr>
								<td colspan="7" nowrap>&nbsp;</td>
							</tr>
							<tr>
								<th align="right" colspan="6" nowrap>Total Frags</th>
								<th align="right"><?php echo $totalfrags; ?></th>
							</tr>
<?php
		# Get the average ping (don't include bots!)
		if($nonbots > 0)
		{
?>
							<tr>
								<th align="right" colspan="6" nowrap>Average Ping</th>
								<th align="right"><?php echo floor($totalping/$nonbots); ?></th>
							</tr>
<?php
		}
?>
						</table>
<?php
	}
}
else
{
	if($server_players > 0)
	{
?>
						<table border="0" width="99%">
							<tr>
								<th align="left" width="25">ID</th>
								<th align="left">Name</th>
								<th align="left" width="40">Frags</th>
								<th align="left" width="75">Connected</th>
							</tr>	
<?php
		$totalfrags = 0;
		$totaltime = 0;
		foreach($server_players as $key => $player)
		{
?>
							<tr>
								<td align="left"><?php echo $player['index']; ?></td>
								<td align="left"><a href="<?php echo $g_options['scripturl']; ?>?mode=search&amp;q=<?php echo $player['name']; ?>&amp;st=player&amp;game=<?php echo $server_details['gamedir']; ?>"><?php echo $player['name']; ?></a></td>
								<td align="right"><?php echo $player['frags']; ?></td>
								<td align="right"><?php echo Formated_Times($player['time']); ?></td>
							</tr>
<?php
			$totalfrags += $player['frags'];
			$totaltime += $player['time'];
		}
?>
							<tr>
								<th align="right" colspan="2" nowrap>Total:</th>
								<th align="right"><?php echo $totalfrags; ?></th>
								<th align="right"><?php echo Formated_Times($totaltime); ?></th>
							</tr>
						</table>
<?php
	}
}
if(count($server_hltv) > 0)
{
?>
						<br><br>
						<table border="0" width="99%">
							<tr>
								<th align="left" width="16"></th>
								<th align="left">Name</th>
								<th align="left" width="125">IP</th>
								<th align="right" width="40">Delay</th>
								<th align="right" width="60">Viewers</th>
								<th align="right" width="60">Capacity</th>
								<th align="right" width="75">Connected</th>
							</tr>	
<?php
	foreach($server_hltv as $hltv)
	{
?>
							<tr>
								<td><img src="<?php echo $g_options['imgdir'];?>/hltv.gif" alt="HLTV"></td>
								<td align="left"><?php echo $hltv['name'];?></td>
								<td align="right"><?php echo $hltv['ip'];?></td>
								<td align="right"><?php echo $hltv['delay'];?></td>
								<td align="right"><?php echo $hltv['viewers'];?></td>
								<td align="right"><?php echo $hltv['capacity'];?></td>
								<td align="right"><?php echo $hltv['time'];?></td>
							</tr>
<?php
	}
?>							
						</table>
<?php
}
if($server_details['players_connecting'] > 0)
{
	echo "There are currently <b>$server_details[players_connecting]</b> player(s) connecting to the server.";
}
?>

					</td>
				</tr>
				<tr>
					<td valign="top" class="fontNormal">
<?php
# For our main server information area we can grab some 
# bits of information from the rules

# If ClanMod or AMX mod is installed we may know what 
# the next map is
if(isset($server_rules['cm_nextmap']))
	$server_details['nextmap'] = $server_rules['cm_nextmap'];
elseif(isset($server_rules['amx_nextmap']))
	$server_details['nextmap'] = $server_rules['amx_nextmap'];

?>

						Address: <?php echo $server_details['address'];?><br>
						Server Type: <?php echo $server_details['os'];?>, <?php echo $server_details['type'];?><br>
						Map: <a href="<?php echo $g_options['scripturl']; ?>?mode=mapinfo&amp;map=<?php echo $server_details['map'];?>&amp;game=<?php echo $server_details['gamedir'];?>"><?php echo $server_details['map'];?></a><br>
<?php
if(isset($server_details['nextmap']))
	echo 'Nextmap: <a href="'.$g_options['scripturl'].'?mode=mapinfo&amp;map='.$server_details['nextmap'].'&amp;game='.$server_details['gamedir'].'">'.$server_details['nextmap'].'</a><br>';

# Some unfortunate games like CS don't usually give the map timeleft
# I wonder if some plugin can yet again provide a use here...
# Are there any time limits or frag limits?
if(!isset($server_rules['mp_timeleft'])||$server_rules['mp_timeleft']==0)
{
	if(isset($server_rules['amx_timeleft']))
		$server_details['timeleft'] = $server_rules['amx_timeleft'];
	if(isset($server_rules['cm_timeleft']))
		$server_details['timeleft'] = $server_rules['cm_timeleft'];
	echo 'Timeleft: '.$server_details['timeleft'];
	if(isset($server_rules['mp_timelimit']))
	{
		echo ' ('.date('i:s', $server_rules['mp_timelimit'] * 60) . ')';
	}
	echo '<br>';
}
else
{
	$server_details['timeleft'] = $server_rules['mp_timeleft'];
	echo 'Timeleft: '.date('i:s', $server_details['timeleft']);
	if(isset($server_rules['mp_timelimit']))
	{
		echo ' ('.date('i:s', $server_rules['mp_timelimit'] * 60) . ')';
	}
	echo '<br>';
}
if($server_rules['mp_fraglimit'] > 0)
{
	echo 'Fragsleft: '.$server_rules['mp_fragsleft'];
	if(isset($server_rules['mp_fragslimit']))
	{
		echo '('.$server_rules['mp_fraglimit'].')';
	}
	echo '<br>';
}

?>
						Password: <?php echo $server_details['password'];?><br>
						Players: <?php echo $server_details['players_real'];?>/<?php echo $server_details['max'];?><br>
<?php
if($server_details['botcount'] > 0)
{
	# Don't show this information if there are no bots
?>
						Bots: <?php echo $server_details['botcount'];?>/<?php echo $server_details['max'];?><br>
<?php
}
if($server_details['hltvcount'] > 0)
{
	# Don't show this information if there is no HLTV
?>
						HLTV: <?php echo $server_details['hltvcount'];?>/<?php echo $server_details['max'];?><br>
<?php
}
if($server_details['players_connecting'] > 0)
{
	# Don't show this information if there are no players connecting
?>
						Connecting: <?php echo $server_details['players_connecting'] ?>/<?php echo $server_details['max'];?><br>
<?php
}
?>
						Valve Anti-Cheat: <?php echo $server_details['secure'];?><br>
<?php
$addon_array = [];
$server_details['addon_count'] = 0;
if($server_rules)
{
?>
						Rules:<br>
						<select name="rules" style="width: 200px;">
<?php
	# Load our plugin list
	$sql2 = $xoopsDB->query('SELECT * FROM ' . $xoopsDB->prefix('hlstats_server_addons') . '');

	while (false !== ($addon_list = $xoopsDB->fetchArray($sql2)))
	{
		$server_addon[$addon_list['rule']] = ['addon' => $addon_list['addon'], 'url' => $addon_list['url']];
	}
	foreach ($server_rules as $key => $value)
	{
		if(isset($server_addon[$key]))
		{
			if($server_addon[$key]['url'])
				$addon_array[] = '<a href="'.$server_addon[$key]['url'].'" target="_blank">'.str_replace('%', $value, $server_addon[$key]['addon']).'</a>';
			else
				$addon_array[] = str_replace('%', $value, $server_addon[$key]['addon']);
			$server_details['addon_count']++;
		}
		echo "<option>$key = $value</option>\n";
	}
?>
						</select><br><br>
<?php
}
?>
<?php
if($server_details['addon_count'] > 0)
{
?>
Server Addons:<br>

<ul>
<?php
	foreach($addon_array as $addon)
	{
?>
	<li> <?php echo $addon; ?></li>

<?php
	}
?>
</ul>
<?php
}
?>		
					</td>
				</tr>
			</table>
		</td>
	</tr>
</table>
