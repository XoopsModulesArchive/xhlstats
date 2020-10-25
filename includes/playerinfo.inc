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
	
	
	
	// Player Details
	
	if(isset($_GET['player'])){
		$player = (int)$_GET["player"];
	}
	if(isset($_GET['uniqueid'])){
		$uniqueid  = (string)$_GET["uniqueid"];
	}
	if(isset($_GET['game'])){
		$game = (string)$_GET["game"];
	}
	
	if (!$player && $uniqueid)
	{
		if (!$game)
		{
			header('Location: ' . $g_options['scripturl'] . "?mode=search&st=uniqueid&q=$uniqueid");
			exit;
		}
		
		$sql = $xoopsDB->query(
            '
			SELECT
				playerId
			FROM
				' . $xoopsDB->prefix('hlstats_PlayerUniqueIds') . "
			WHERE
				uniqueId='$uniqueid'
				AND game='$game'
		");
		
		if ($xoopsDB->getRowsNum($sql) > 1)
		{
			header('Location: ' . $g_options['scripturl'] . "?mode=search&st=uniqueid&q=$uniqueid&game=$game");
			exit;
		}
		elseif ($xoopsDB->getRowsNum($sql) < 1)
		{
			error("No players found matching uniqueId '$uniqueid'");
		}
		else
		{
			[$player] = $xoopsDB->fetchRow($sql);
			$player = (int)$player;
		}
	}
	elseif (!$player && !$uniqueid)
	{
		error('No player ID specified.');
	}
	
	$sql1 = $xoopsDB->query(
        '
		SELECT
			' . $xoopsDB->prefix('hlstats_Players') . '.lastName,
			' . $xoopsDB->prefix('hlstats_Players') . '.clan,
			' . $xoopsDB->prefix('hlstats_Players') . '.fullName,
			' . $xoopsDB->prefix('hlstats_Players') . '.email,
			' . $xoopsDB->prefix('hlstats_Players') . '.homepage,
			' . $xoopsDB->prefix('hlstats_Players') . '.icq,
			' . $xoopsDB->prefix('hlstats_Players') . '.game,
			' . $xoopsDB->prefix('hlstats_Players') . '.skill,
			' . $xoopsDB->prefix('hlstats_Players') . '.kills,
			' . $xoopsDB->prefix('hlstats_Players') . ".deaths,
			IFNULL(kills/deaths, '-') AS kpd,
			" . $xoopsDB->prefix('hlstats_Players') . '.suicides,
			CONCAT(' . $xoopsDB->prefix('hlstats_Clans') . ".tag, ' ', " . $xoopsDB->prefix('hlstats_Clans') . '.name) AS clan_name
		FROM
			' . $xoopsDB->prefix('hlstats_Players') . '
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Clans') . ' ON
			' . $xoopsDB->prefix('hlstats_Clans') . '.clanId = ' . $xoopsDB->prefix('hlstats_Players') . ".clan
		WHERE
			playerId='$player'
	");
	if ($xoopsDB->getRowsNum($sql1) != 1)
		error("No such player '$player'.");
	
	$playerdata = $xoopsDB->fetchArray($sql1);
	$xoopsDB->freeRecordSet($sql1);
	
	$pl_name = $playerdata['lastName'];
	if (strlen($pl_name) > 10)
	{
		$pl_shortname = substr($pl_name, 0, 8) . '...';
	}
	else
	{
		$pl_shortname = $pl_name;
	}
	$pl_name = ereg_replace(' ', '&nbsp;', htmlspecialchars($pl_name, ENT_QUOTES | ENT_HTML5));
	$pl_shortname = ereg_replace(' ', '&nbsp;', htmlspecialchars($pl_shortname, ENT_QUOTES | ENT_HTML5));
	$pl_urlname = urlencode($playerdata['lastName']);
	
	
	$game = $playerdata['game'];
	$sql2 = $xoopsDB->query('SELECT name FROM ' . $xoopsDB->prefix('hlstats_Games') . " WHERE code='$game'");
	if ($xoopsDB->getRowsNum($sql2) != 1)
		$gamename = ucfirst($game);
	else
		[$gamename] = $xoopsDB->fetchRow($sql2);
	
	
	pageHeader(
        [$gamename, 'Player Details', $pl_name],
        [
            $gamename         => $g_options['scripturl'] . "?game=$game",
            'Player Rankings' => $g_options['scripturl'] . "?mode=players&game=$game",
            'Player Details'  => ''
		],
        $pl_name
	);
?>



<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="60%" colspan=2><font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Player Profile</b><?php echo '</font>';?></td>
	<td width="40%" colspan=2><font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Statistics Summary</b><?php echo '</font>';?></td>
</tr>

<tr valign="top">
	<td width="5%">&nbsp;</td>
	<td width="50%">&nbsp;<br>
		<table width="95%" border=0 cellspacing=0 cellpadding=0>
		<tr>
			<td>
				<table width="100%" border=0 cellspacing=1 cellpadding=4>
			
				<tr>
					<td><?php
						echo '<font class="fontNormal">';
						echo 'Member of Clan:';
						echo '</font>';
					?></td>
					<td><?php
						echo '<font class="fontNormal">';
						if ($playerdata['clan'])
						{
							echo '&nbsp;<a href="' . $g_options['scripturl']
                                 . '?mode=claninfo&clan=' . $playerdata['clan']
                                 . '"><img src="' . $g_options['imgdir']
                                 . '/clan.gif" width=16 height=16 hspace=4 '
                                 . 'border=0 align="middle" alt="clan.gif">'
                                 . htmlspecialchars($playerdata['clan_name'], ENT_QUOTES | ENT_HTML5) . '</a>';
						}
						else
						{
							echo '(None.)';
						}
						echo '</font>';
					?></td>
				</tr>
	
				<tr>
					<td><?php
						echo '<font class="fontNormal">';
						echo 'Real Name:';
						echo '</font>';
					?></td>
					<td><?php
						echo '<font class="fontNormal">';
						if ($playerdata['fullName'])
						{
							echo '<b>' . htmlspecialchars($playerdata['fullName'], ENT_QUOTES | ENT_HTML5) . '</b>';
						}
						else
						{
							echo '(Unknown.)';
						}
						echo '</font>';
					?></td>
				</tr>
		
				<tr>
					<td><?php
						echo '<font class="fontNormal">';
						echo 'E-mail Address:';
						echo '</font>';
					?></td>
					<td><?php
						echo '<font class="fontNormal">';
						if ($email = getEmailLink($playerdata['email']))
						{
							echo $email;
						}
						else
						{
							echo '(Unknown.)';
						}
						echo '</font>';
					?></td>
				</tr>
				
				<tr>
					<td><?php
						echo '<font class="fontNormal">';
						echo 'Home Page:';
						echo '</font>';
					?></td>
					<td><?php
						echo '<font class="fontNormal">';
						if ($url = getLink($playerdata['homepage']))
						{
							echo $url;
						}
						else
						{
							echo '(Not specified.)';
						}
						echo '</font>';
					?></td>
				</tr>
				
				<tr>
					<td><?php
						echo '<font class="fontNormal">';
						echo 'ICQ Number:';
						echo '</font>';
					?></td>
					<td><?php
						echo '<font class="fontNormal">';
						if ($playerdata['icq'])
						{
							echo '<a href="http://wwp.icq.com/'
                                 . urlencode($playerdata['icq']) . '" target="_blank">'
                                 . htmlspecialchars($playerdata['icq'], ENT_QUOTES | ENT_HTML5) . '</a>';
						}
						else
						{
							echo '(Not specified.)';
						}
						echo '</font>';
					?></td>
				</tr>
				
				<tr>
					<td><?php
						echo '<font class="fontNormal">';
						echo 'Player ID:';
						echo '</font>';
					?></td>
					<td><?php
						echo '<font class="fontNormal">';
						echo $player;
						echo '</font>';
					?></td>
				</tr>

				<tr>
					<td><?php
						echo '<font class="fontNormal">';
						if (MODE == 'LAN')
						{
							echo 'IP Addresses:';
						}
						else
						{
							echo 'Unique IDs:';
						}
						echo '</font>';
					?></td>
					<td><?php
						echo '<font class="fontNormal">';
						if (MODE == 'NameTrack')
						{
							echo '(Unknown.)';
						}
						else
						{
							$sql3 = $xoopsDB->query(
                                '
								SELECT
									uniqueId
								FROM
									' . $xoopsDB->prefix('hlstats_PlayerUniqueIds') . "
								WHERE
									playerId='$player'
							");
							
							$i=0;
							while (list($uqid) = $xoopsDB->fetchRow($sql3))
							{
								if ($i > 0) echo ', ';
								echo $uqid;
								$i++;
							}
						}
						
						echo '</font>';
					?></td>
				</tr>
				
				<tr>
					<td><?php
						echo '<font class="fontNormal">';
						echo 'Last Connect:*';
						echo '</font>';
					?></td>
					<td><?php
						echo '<font class="fontNormal">';
						$sql4 = $xoopsDB->query("
							SELECT
								DATE_FORMAT(MAX(eventTime), '%r, %a. %D %b.')
							FROM
								".$xoopsDB->prefix('hlstats_Events_Connects') . '
							LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
								' . $xoopsDB->prefix('hlstats_Servers') . '.serverId=' . $xoopsDB->prefix('hlstats_Events_Connects') . '.serverId
							WHERE
								' . $xoopsDB->prefix('hlstats_Servers') . ".game='$game' AND playerId='$player'
						");
						[$lastevent] = $xoopsDB->fetchRow($sql4);
						
						if ($lastevent)
						{
							echo $lastevent;
						}
						else
						{
							echo '(Unknown.)';
						}
				?></font></td>
				</tr>

				<tr>
					<td><?php
						echo '<font class="fontNormal">';
						echo 'Total Connection Time:*';
						echo '</font>';
					?></td>
					<td><?php
						echo '<font class="fontNormal">';
						$sql5 = $xoopsDB->query(
                            '
							SELECT  
								SEC_TO_TIME(SUM(TIME_TO_SEC(time))) AS tTime
							FROM
								' . $xoopsDB->prefix('hlstats_Events_StatsmeTime') . '
							LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
								' . $xoopsDB->prefix('hlstats_Servers') . '.serverId=' . $xoopsDB->prefix('hlstats_Events_StatsmeTime') . '.serverId
							WHERE   
								' . $xoopsDB->prefix('hlstats_Servers') . ".game='$game' AND playerId='$player'
						");
						[$tTime] = $xoopsDB->fetchRow($sql5);

						if ($tTime)
						{
							echo $tTime;
						}
						else
						{
							echo '(Unknown.)';
						}
				?></font></td>
				</tr>

				<tr>
					<td><?php
						echo '<font class="fontNormal">';
						echo 'Average Ping:*';
						echo '</font>';
					?></td>
					<td><?php
						echo '<font class="fontNormal">';
						$sql6 = $xoopsDB->query(
                            '
							SELECT
								ROUND(SUM(ping) /
									COUNT(ping), 1) AS av_ping
							FROM
								' . $xoopsDB->prefix('hlstats_Events_StatsmeLatency') . '
							LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
								' . $xoopsDB->prefix('hlstats_Servers') . '.serverId=' . $xoopsDB->prefix('hlstats_Events_StatsmeLatency') . '.serverId
							WHERE   
								' . $xoopsDB->prefix('hlstats_Servers') . ".game='$game' AND playerId='$player'
						");
						[$av_ping] = $xoopsDB->fetchRow($sql6);

						if ($av_ping)
						{
							echo $av_ping;
						}
						else
						{  
							echo '(Unknown.)';
						}
				?></font></td>
				</tr>

				</table></td>
		</tr>
		
		</table></td>
	<td width="5%">&nbsp;</td>
	<td width="40%">&nbsp;<br>
		<table width="100%" border=0 cellspacing=0 cellpadding=0>

		<tr>
			<td>
				<table width="100%" border=0 cellspacing=1 cellpadding=4>
			
				<tr>
					<td width="45%"><?php
						echo '<font class="fontNormal">';
						echo 'Points:';
						echo '</font>';
					?></td>
					<td width="55%"><?php
						echo '<font class="fontNormal">';
						echo '<b>' . $playerdata['skill'] . '</b>';
						echo '</font>';
					?></td>
				</tr>
				
				<tr>
					<td width="45%"><?php
						echo '<font class="fontNormal">';
						echo 'Kills:';
						echo '</font>';
					?></td>
					<td width="55%"><?php
						echo '<font class="fontNormal">';
						echo $playerdata['kills'];
						$sql7 = $xoopsDB->query(
                            '
							SELECT
								COUNT(*)
							FROM
								' . $xoopsDB->prefix('hlstats_Events_Frags') . '
							LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
								' . $xoopsDB->prefix('hlstats_Servers') . '.serverId=' . $xoopsDB->prefix('hlstats_Events_Frags') . '.serverId
							WHERE
								' . $xoopsDB->prefix('hlstats_Servers') . ".game='$game' AND killerId='$player'
						");
						[$realkills] = $xoopsDB->fetchRow($sql7);
						echo " ($realkills)";
						echo '</font>';
					?></td>
				</tr>
				
				<tr>
					<td width="45%"><?php
						echo '<font class="fontNormal">';
						echo 'Deaths:';
						echo '</font>';
					?></td>
					<td width="55%"><?php
						echo '<font class="fontNormal">';
						echo $playerdata['deaths'];
						echo '</font>';
					?></td>
				</tr>
				
				<tr>
					<td width="45%"><?php
						echo '<font class="fontNormal">';
						echo 'Suicides:';
						echo '</font>';
					?></td>
					<td width="55%"><?php
						echo '<font class="fontNormal">';
						echo $playerdata['suicides'];
						echo '</font>';
					?></td>
				</tr>
				
				<tr>
					<td width="45%"><?php
						echo '<font class="fontNormal">';
						echo 'Kills per Death:';
						echo '</font>';
					?></td>
					<td width="55%"><?php
						echo '<font class="fontNormal">';
						echo $playerdata['kpd'];
						echo '</font>';
					?></td>
				</tr>
				
				<tr>
					<td width="45%"><?php
						echo '<font class="fontNormal">';
						echo 'Teammate Kills:*';
						echo '</font>';
					?></td>
					<td width="55%"><?php
						echo '<font class="fontNormal">';
						
						$sql8 = $xoopsDB->query(
                            '
							SELECT
								COUNT(*)
							FROM
								' . $xoopsDB->prefix('hlstats_Events_Teamkills') . '
							LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
								' . $xoopsDB->prefix('hlstats_Servers') . '.serverId=' . $xoopsDB->prefix('hlstats_Events_Teamkills') . '.serverId
							WHERE
								' . $xoopsDB->prefix('hlstats_Servers') . ".game='$game' AND killerId='$player'
						");
						[$playerdata['teamkills']] = $xoopsDB->fetchRow($sql8);
						
						echo $playerdata['teamkills'];
						echo '</font>';
					?></td>
				</tr>

				<tr>
					<td width="45%"><?php
						echo '<font class="fontNormal">';
						echo 'Weapon Accuracy:';
						echo '</font>';
					?></td>
					<td width="55%"><?php
						echo '<font class="fontNormal">';

						$sql9 = $xoopsDB->query(
                            '
							SELECT
								IFNULL(ROUND((SUM(' . $xoopsDB->prefix('hlstats_Events_Statsme') . '.hits)
									/ SUM(' . $xoopsDB->prefix('hlstats_Events_Statsme') . '.shots) * 100), 1), 0.0) AS accuracy
							FROM
								' . $xoopsDB->prefix('hlstats_Events_Statsme') . '
							LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
								' . $xoopsDB->prefix('hlstats_Servers') . '.serverId=' . $xoopsDB->prefix('hlstats_Events_Statsme') . '.serverId
							WHERE
								' . $xoopsDB->prefix('hlstats_Servers') . ".game='$game' AND playerId='$player'
						");
						[$playerdata['accuracy']] = $xoopsDB->fetchRow($sql9);

						if ($playerdata['accuracy'] == 0)
						{
							echo '(Unknown.)';
						}
						else
						{
							echo $playerdata['accuracy'] . '%';
						}
						echo '</font>';
					?></td>
				</tr>

				</table></td>
		</tr>
		
		</table><br>
		
		
		<?php
			echo '<font class="fontNormal">';
			
			echo '&nbsp;<a href="' . $g_options['scripturl']
                 . "?mode=playerhistory&player=$player\"><img "
                 . 'src="' . $g_options['imgdir']
                 . '/history.gif" width=16 height=16 border=0 '
                 . 'hspace=3 align="middle" alt="history.gif">'
                 . htmlspecialchars($playerdata['lastName'], ENT_QUOTES | ENT_HTML5) . "'s Event&nbsp;History</a><br>\n";
			
		?>&nbsp;<a href="<?php echo $g_options['scripturl']; ?>?mode=search&st=player&q=<?php echo $pl_urlname; ?>"><img src="<?php echo $g_options['imgdir']; ?>/search.gif" width=16 height=16 hspace=3 border=0 align="middle" alt="search.gif">Find other players with the same name</a></font></td>
</tr>

</table><p>

<?php
	flush();
	
	$tblAliases = new Table(
		[
			new TableColumn(
                'name', 'Name', 'width=25'
			),
			new TableColumn(
                'numuses', 'Used', 'width=10&align=right&append=+times'
			),
			new TableColumn(
                'lastuse', 'Last Use', 'width=20'
			),
			new TableColumn(
                'kills', 'Kills', 'width=10&align=right'
			),
			new TableColumn(
                'deaths', 'Deaths', 'width=10&align=right'
			),
			new TableColumn(
                'kpd', 'Kills per Death', 'width=10&align=right'
			),
			new TableColumn(
                'suicides', 'Suicides', 'width=10&align=right'
			)
		], 'name', 'lastuse', 'name',
		true,
		20, 'aliases_page', 'aliases_sort', 'aliases_sortorder', 'aliases'
	);

	$result = $xoopsDB->query("
		SELECT
			name,
			lastuse,
			numuses,
			kills,
			deaths,
			IFNULL(
				kills / deaths,
				'-'
			) AS kpd,
			suicides
		FROM
			".$xoopsDB->prefix('hlstats_PlayerNames')."
		WHERE
			playerId=$player
		ORDER BY
			$tblAliases->sort $tblAliases->sortorder
		LIMIT $tblAliases->startitem,$tblAliases->numperpage
	");
	
	$resultCount = $xoopsDB->query(
        '
		SELECT
			COUNT(*)
		FROM
			' . $xoopsDB->prefix('hlstats_PlayerNames') . "
		WHERE
			playerId=$player
	");
	
	[$numitems] = $xoopsDB->fetchRow($resultCount);

	if ($numitems > 1)
	{
?>

<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="100%" colspan=3><a name="aliases"></a>
<font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Aliases</b><?php echo '</font>';?></td>
</tr>

<tr>
	<td width="5%">&nbsp;</td>
	<td width="95%" colspan=2>&nbsp;<br>
	<?php
		$tblAliases->draw($result, $numitems, 100);
	?></td>
</tr>

</table><p>
<?php
	}

	flush();
	
	$tblPlayerActions = new Table(
		[
			new TableColumn(
                'description', 'Action', 'width=45'
			),
			new TableColumn(
                'obj_count', 'Achieved', 'width=25&align=right&append=+times'
			),
			new TableColumn(
                'obj_bonus', 'Points Bonus', 'width=25&align=right'
			)
		], 'id', 'obj_count', 'description',
		true,
		9999, 'obj_page', 'obj_sort', 'obj_sortorder', 'playeractions'
	);

	$result = $xoopsDB->query(
        '
		SELECT
			' . $xoopsDB->prefix('hlstats_Actions') . '.description,
			COUNT(' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . '.id) AS obj_count,
			COUNT(' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . '.id) * ' . $xoopsDB->prefix('hlstats_Actions') . '.reward_player AS obj_bonus
		FROM
			' . $xoopsDB->prefix('hlstats_Actions') . '
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . ' ON
			' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . '.actionId = ' . $xoopsDB->prefix('hlstats_Actions') . '.id
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . '.serverId=' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . '.serverId
		WHERE
			' . $xoopsDB->prefix('hlstats_Servers') . ".game='$game' AND " . $xoopsDB->prefix('hlstats_Events_PlayerActions') . ".playerId=$player
		GROUP BY
			" . $xoopsDB->prefix('hlstats_Actions') . ".id
		ORDER BY
			$tblPlayerActions->sort $tblPlayerActions->sortorder,
			$tblPlayerActions->sort2 $tblPlayerActions->sortorder
	");
	
	$numitems = $xoopsDB->getRowsNum($result);
	
	if ($numitems > 0)
	{
?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="50%" colspan=2><a name="playeractions"></a>
<font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Player Actions</b><?php echo '</font>';?></td>
	<td width="50%" align="right"><font class="fontNormal">(Last <?php echo DELETEDAYS; ?> Days)<?php echo '</font>';?></td>
</tr>

<tr>
	<td width="5%">&nbsp;</td>
	<td width="95%" colspan=2>&nbsp;<br>
	<?php
		$tblPlayerActions->draw($result, $numitems, 100);
	?></td>
</tr>

</table><p>
<?php
	}
	
	
	$tblPlayerPlayerActions = new Table(
		[
			new TableColumn(
                'description', 'Action', 'width=45'
			),
			new TableColumn(
                'obj_count', 'Achieved', 'width=25&align=right&append=+times'
			),
			new TableColumn(
                'obj_bonus', 'Points Bonus', 'width=25&align=right'
			)
		], 'id', 'obj_count', 'description',
		true,
		9999, 'ppa_page', 'ppa_sort', 'ppa_sortorder', 'playerplayeractions'
	);

	$result = $xoopsDB->query(
        '
		SELECT
			' . $xoopsDB->prefix('hlstats_Actions') . '.description,
			COUNT(' . $xoopsDB->prefix('hlstats_Events_PlayerPlayerActions') . '.id) AS obj_count,
			COUNT(' . $xoopsDB->prefix('hlstats_Events_PlayerPlayerActions') . '.id) * ' . $xoopsDB->prefix('hlstats_Actions') . '.reward_player AS obj_bonus
		FROM
			' . $xoopsDB->prefix('hlstats_Actions') . '
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Events_PlayerPlayerActions') . ' ON
			' . $xoopsDB->prefix('hlstats_Events_PlayerPlayerActions') . '.actionId = ' . $xoopsDB->prefix('hlstats_Actions') . '.id
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . '.serverId=' . $xoopsDB->prefix('hlstats_Events_PlayerPlayerActions') . '.serverId
		WHERE
			' . $xoopsDB->prefix('hlstats_Servers') . ".game='$game' AND " . $xoopsDB->prefix('hlstats_Events_PlayerPlayerActions') . ".playerId=$player
		GROUP BY
			" . $xoopsDB->prefix('hlstats_Actions') . ".id
		ORDER BY
			$tblPlayerPlayerActions->sort $tblPlayerPlayerActions->sortorder,
			$tblPlayerPlayerActions->sort2 $tblPlayerPlayerActions->sortorder
	");
	
	$numitems = $xoopsDB->getRowsNum($result);
	
	if ($numitems > 0)
	{
?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="50%" colspan=2><a name="playerplayeractions"></a>
<font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Player-Player Actions</b><?php echo '</font>';?></td>
	<td width="50%" align="right"><font class="fontNormal">(Last <?php echo DELETEDAYS; ?> Days)<?php echo '</font>';?></td>
</tr>

<tr>
	<td width="5%">&nbsp;</td>
	<td width="95%" colspan=2>&nbsp;<br>
	<?php
		$tblPlayerPlayerActions->draw($result, $numitems, 100);
	?></td>
</tr>

</table><p>
<?php
	}
	
	
	flush();
	
	$tblTeams = new Table(
        [
			new TableColumn(
                'name', 'Team', 'width=35'
			),
			new TableColumn(
                'teamcount', 'Joined', 'width=10&align=right&append=+times'
			),
			new TableColumn(
                'percent', 'Percentage of Times', 'width=40&sort=no&type=bargraph'
			),
			new TableColumn(
                'percent', '%',
                'width=10&sort=no&align=right&append=' . urlencode('%')
			)
		], 'name', 'teamcount', 'name', true,
        9999, 'teams_page', 'teams_sort', 'teams_sortorder', 'teams'
	);
	
	$sql10 = $xoopsDB->query(
        '
		SELECT
			COUNT(*)
		FROM
			' . $xoopsDB->prefix('hlstats_Events_ChangeTeam') . "
		WHERE
			playerId=$player
	");
	[$numteamjoins] = $xoopsDB->fetchRow($sql10);

	$result = $xoopsDB->query(
        '
		SELECT
			IFNULL(' . $xoopsDB->prefix('hlstats_Teams') . '.name, ' . $xoopsDB->prefix('hlstats_Events_ChangeTeam') . '.team) AS name,
			COUNT(' . $xoopsDB->prefix('hlstats_Events_ChangeTeam') . '.id) AS teamcount,
			COUNT(' . $xoopsDB->prefix('hlstats_Events_ChangeTeam') . ".id) / $numteamjoins * 100 AS percent
		FROM
			" . $xoopsDB->prefix('hlstats_Events_ChangeTeam') . '
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Teams') . ' ON
			' . $xoopsDB->prefix('hlstats_Events_ChangeTeam') . '.team=' . $xoopsDB->prefix('hlstats_Teams') . '.code
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . '.serverId=' . $xoopsDB->prefix('hlstats_Events_ChangeTeam') . '.serverId
		WHERE
			' . $xoopsDB->prefix('hlstats_Servers') . ".game='$game' AND " . $xoopsDB->prefix('hlstats_Events_ChangeTeam') . ".playerId=$player AND (hidden <>'1' OR hidden IS NULL)
		GROUP BY
			" . $xoopsDB->prefix('hlstats_Events_ChangeTeam') . ".team
		ORDER BY
			$tblTeams->sort $tblTeams->sortorder,
			$tblTeams->sort2 $tblTeams->sortorder
	");
	
	$numitems = $xoopsDB->getRowsNum($result);
	
	if ($numitems > 0)
	{
?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="50%" colspan=2><a name="teams"></a>
<font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Team Selection</b><?php echo '</font>';?></td>
	<td width="50%" align="right"><font class="fontNormal">(Last <?php echo DELETEDAYS; ?> Days)<?php echo '</font>';?></td>
</tr>

<tr>
	<td width="5%">&nbsp;</td>
	<td width="95%" colspan=2>&nbsp;<br>
	<?php
		$tblTeams->draw($result, $numitems, 100);
	?></td>
</tr>

</table><p>
<?php
	}
	
	
	flush();
	
	$tblRoles = new Table(
        [
			new TableColumn(
                'name', 'Role', 'width=35'
			),
			new TableColumn(
                'rolecount', 'Joined', 'width=10&align=right&append=+times'
			),
			new TableColumn(
                'percent', 'Percentage of Times', 'width=40&sort=no&type=bargraph'
			),
			new TableColumn(
                'percent', '%',
                'width=10&sort=no&align=right&append=' . urlencode('%')
			)
		], 'name', 'rolecount', 'name', true,
        9999, 'roles_page', 'roles_sort', 'roles_sortorder', 'roles'
	);
	
	$sql11 = $xoopsDB->query(
        '
		SELECT
			COUNT(*)
		FROM
			' . $xoopsDB->prefix('hlstats_Events_ChangeRole') . "
		WHERE
			playerId=$player
	");
	[$numrolejoins] = $xoopsDB->fetchRow($sql11);

	$result = $xoopsDB->query(
        '
		SELECT
			IFNULL(' . $xoopsDB->prefix('hlstats_Roles') . '.name, ' . $xoopsDB->prefix('hlstats_Events_ChangeRole') . '.role) AS name,
			COUNT(' . $xoopsDB->prefix('hlstats_Events_ChangeRole') . '.id) AS rolecount,
			COUNT(' . $xoopsDB->prefix('hlstats_Events_ChangeRole') . ".id) / $numrolejoins * 100 AS percent
		FROM
			" . $xoopsDB->prefix('hlstats_Events_ChangeRole') . '
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Roles') . ' ON
			' . $xoopsDB->prefix('hlstats_Events_ChangeRole') . '.role=' . $xoopsDB->prefix('hlstats_Roles') . '.code
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . '.serverId=' . $xoopsDB->prefix('hlstats_Events_ChangeRole') . '.serverId
		WHERE
			' . $xoopsDB->prefix('hlstats_Servers') . ".game='$game' AND " . $xoopsDB->prefix('hlstats_Events_ChangeRole') . ".playerId=$player AND (hidden <>'1' OR hidden IS NULL)
		GROUP BY
			" . $xoopsDB->prefix('hlstats_Events_ChangeRole') . ".role
		ORDER BY
			$tblRoles->sort $tblRoles->sortorder,
			$tblRoles->sort2 $tblRoles->sortorder
	");
	
	$numitems = $xoopsDB->getRowsNum($result);
	
	if ($numitems > 0)
	{
?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="50%" colspan=2><a name="roles"></a>
<font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Role Selection</b><?php echo '</font>';?></td>
	<td width="50%" align="right"><font class="fontNormal">(Last <?php echo DELETEDAYS; ?> Days)<?php echo '</font>';?></td>
</tr>

<tr>
	<td width="5%">&nbsp;</td>
	<td width="95%" colspan=2>&nbsp;<br>
	<?php
		$tblRoles->draw($result, $numitems, 100);
	?></td>
</tr>

</table><p>

<?php
	}
	
	
	flush();
	
	$tblWeapons = new Table(
		[
			new TableColumn(
                'weapon', 'Weapon',
                'width=21&type=weaponimg&align=center&link=' . urlencode("mode=weaponinfo&weapon=%k&game=$game")
			),
			new TableColumn(
                'modifier', 'Points Modifier', 'width=10&align=right'
			),
			new TableColumn(
                'kills', 'Kills', 'width=12&align=right'
			),
			new TableColumn(
                'percent', 'Percentage of Kills', 'width=40&sort=no&type=bargraph'
			),
			new TableColumn(
                'percent', '%',
                'width=12&sort=no&align=right&append=' . urlencode('%')
			)
		], 'weapon', 'kills', 'weapon',
		true, 9999, 'weap_page', 'weap_sort', 'weap_sortorder', 'weapons'
	);
	
	$result = $xoopsDB->query(
        '
		SELECT
			' . $xoopsDB->prefix('hlstats_Events_Frags') . '.weapon,
			IFNULL(' . $xoopsDB->prefix('hlstats_Weapons') . '.modifier, 1.00) AS modifier,
			COUNT(' . $xoopsDB->prefix('hlstats_Events_Frags') . '.weapon) AS kills,
			COUNT(' . $xoopsDB->prefix('hlstats_Events_Frags') . ".weapon) / $realkills * 100 AS percent
		FROM
			" . $xoopsDB->prefix('hlstats_Events_Frags') . '
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Weapons') . ' ON
			' . $xoopsDB->prefix('hlstats_Weapons') . '.code = ' . $xoopsDB->prefix('hlstats_Events_Frags') . '.weapon
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . '.serverId=' . $xoopsDB->prefix('hlstats_Events_Frags') . '.serverId
		WHERE
			' . $xoopsDB->prefix('hlstats_Servers') . ".game='$game' AND " . $xoopsDB->prefix('hlstats_Events_Frags') . ".killerId=$player
			AND (" . $xoopsDB->prefix('hlstats_Weapons') . ".game='$game' OR " . $xoopsDB->prefix('hlstats_Weapons') . '.weaponId IS NULL)
		GROUP BY
			' . $xoopsDB->prefix('hlstats_Events_Frags') . ".weapon
		ORDER BY
			$tblWeapons->sort $tblWeapons->sortorder,
			$tblWeapons->sort2 $tblWeapons->sortorder
	");
?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="50%" colspan=2><a name="weapons"></a>
<font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Weapon Usage</b><?php echo '</font>';?></td>
	<td width="50%" align="right"><font class="fontNormal">(Last <?php echo DELETEDAYS; ?> Days)<?php echo '</font>';?></td>
</tr>

<tr>
	<td width="5%">&nbsp;</td>
	<td width="95%" colspan=2>&nbsp;<br>
	<?php
		$tblWeapons->draw($result, $xoopsDB->getRowsNum($result), 100);
	?></td>
</tr>

</table><p>
<!-- Begin StatsMe Addon 1.0 by JustinHoMi@aol.com -->
<?php
	
	flush();

	$tblWeaponstats = new Table(
        [
			new TableColumn(
                'smweapon', 'Weapon',
                'width=21&type=weaponimg&align=center&link=' . urlencode("mode=weaponinfo&weapon=%k&game=$game")
			),
			new TableColumn(
                'smshots', 'Shots', 'width=7&align=right'
			),
			new TableColumn(
                'smhits', 'Hits', 'width=7&align=right'
			),
			new TableColumn(
                'smdamage', 'Damage', 'width=8&align=right'
			),
			new TableColumn(
                'smheadshots', 'Head Shots', 'width=6&align=right'
			),
			new TableColumn(
                'smkills', 'Kills', 'width=7&align=right'
			),
			new TableColumn(
                'smdeaths', 'Deaths', 'width=7&align=right'
			),
			new TableColumn(
                'smkdr', 'Kills per Death', 'width=8&align=right'
			),
			new TableColumn(
                'smaccuracy', 'Accuracy',
                'width=9&align=right&append=' . urlencode('%')
			),
			new TableColumn(
                'smdhr', 'Damage per Hit', 'width=7&align=right'
			),
			new TableColumn(
                'smspk', 'Shots per Kill', 'width=8&align=right'
			)
		], 'smweapon', 'smkdr', 'smweapon', true,
        9999, 'weap_page', 'weap_sort', 'weap_sortorder', 'weaponstats'
	);
	
	$result = $xoopsDB->query(
        '
		SELECT
			' . $xoopsDB->prefix('hlstats_Events_Statsme') . '.weapon AS smweapon,
			SUM(' . $xoopsDB->prefix('hlstats_Events_Statsme') . '.kills) AS smkills,
			SUM(' . $xoopsDB->prefix('hlstats_Events_Statsme') . '.hits) AS smhits,
			SUM(' . $xoopsDB->prefix('hlstats_Events_Statsme') . '.shots) AS smshots,
			SUM(' . $xoopsDB->prefix('hlstats_Events_Statsme') . '.headshots) AS smheadshots,
			SUM(' . $xoopsDB->prefix('hlstats_Events_Statsme') . '.deaths) AS smdeaths,
			SUM(' . $xoopsDB->prefix('hlstats_Events_Statsme') . '.damage) AS smdamage,
			IFNULL((ROUND((SUM(' . $xoopsDB->prefix('hlstats_Events_Statsme') . '.damage) / SUM(' . $xoopsDB->prefix('hlstats_Events_Statsme') . ".hits)), 1)), '-') as smdhr,
			SUM(" . $xoopsDB->prefix('hlstats_Events_Statsme') . '.kills) / IF((SUM(' . $xoopsDB->prefix('hlstats_Events_Statsme') . '.deaths)=0), 1, (SUM(' . $xoopsDB->prefix('hlstats_Events_Statsme') . '.deaths))) as smkdr,
			ROUND((SUM(' . $xoopsDB->prefix('hlstats_Events_Statsme') . '.hits) / SUM(' . $xoopsDB->prefix('hlstats_Events_Statsme') . '.shots) * 100), 1) as smaccuracy,
			IFNULL((ROUND((SUM(' . $xoopsDB->prefix('hlstats_Events_Statsme') . '.shots) / SUM(' . $xoopsDB->prefix('hlstats_Events_Statsme') . ".kills)), 1)), '-') as smspk
		FROM
			" . $xoopsDB->prefix('hlstats_Events_Statsme') . '
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . '.serverId=' . $xoopsDB->prefix('hlstats_Events_Statsme') . '.serverId
		WHERE
			' . $xoopsDB->prefix('hlstats_Servers') . ".game='$game' AND " . $xoopsDB->prefix('hlstats_Events_Statsme') . ".PlayerId=$player
		GROUP BY
			" . $xoopsDB->prefix('hlstats_Events_Statsme') . ".weapon
		ORDER BY
			$tblWeaponstats->sort $tblWeaponstats->sortorder,
			$tblWeaponstats->sort2 $tblWeaponstats->sortorder
	");	

if ($xoopsDB->getRowsNum($result) != 0)
{
?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="50%" colspan=2><a name="weaponstats"></a>
<font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Weapon Stats</b><?php echo '</font>';?></td>
	<td width="50%" align="right"><font class="fontNormal">(Last <?php echo DELETEDAYS; ?> Days)<?php echo '</font>';?></td>
</tr>

<tr>
	<td width="5%">&nbsp;</td>
	<td width="95%" colspan=2>&nbsp;<br>
	<?php
		$tblWeaponstats->draw($result, $xoopsDB->getRowsNum($result), 100);
	?></td>
</tr>

</table><p>
<!-- End StatsMe Addon 1.0 by JustinHoMi@aol.com -->
<?php
}
	
	flush();

	$tblWeaponstats2 = new Table(
        [
			new TableColumn(
                'smweapon', 'Weapon',
                'width=21&type=weaponimg&align=center&link=' . urlencode("mode=weaponinfo&weapon=%k&game=$game")
			),
			new TableColumn(
                'smhead', 'Head', 'width=7&align=right'
			),
			new TableColumn(
                'smchest', 'Chest', 'width=7&align=right'
			),
			new TableColumn(
                'smstomach', 'Stomach', 'width=7&align=right'
			),
			new TableColumn(
                'smleftarm', 'Left Arm', 'width=7&align=right'
			),
			new TableColumn(
                'smrightarm', 'Right Arm', 'width=7&align=right'
			),
			new TableColumn(
                'smleftleg', 'Left Leg', 'width=7&align=right'
			),
			new TableColumn(
                'smrightleg', 'Right Leg', 'width=7&align=right'
			),
			new TableColumn(
                'smleft', 'Left',
                'width=8&align=right&append=' . urlencode('%')
			),
			new TableColumn(
                'smmiddle', 'Middle',
                'width=9&align=right&append=' . urlencode('%')
			),
			new TableColumn(
                'smright', 'Right',
                'width=8&align=right&append=' . urlencode('%')
			)
		], 'smweapon', 'smhead', 'smweapon', true,
        9999, 'weap_page', 'weap_sort', 'weap_sortorder', 'weaponstats2'
	);

	$result = $xoopsDB->query(
        '
		SELECT
			'
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.weapon AS smweapon,
			SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.head) AS smhead,
			SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.chest) AS smchest,
			SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.stomach) AS smstomach,
			SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.leftarm) AS smleftarm,
			SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.rightarm) AS smrightarm,
			SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.leftleg) AS smleftleg,
			SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.rightleg) AS smrightleg,
			IFNULL(ROUND((SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.leftarm) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.leftleg)) / (SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.head) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.chest) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.stomach) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.leftarm ) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.rightarm) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.leftleg) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.rightleg)) * 100, 1), 0.0) AS smleft,
			IFNULL(ROUND((SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.rightarm) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.rightleg)) / (SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.head) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.chest) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.stomach) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.leftarm ) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.rightarm) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.leftleg) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.rightleg)) * 100, 1), 0.0) AS smright,
			IFNULL(ROUND((SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.head) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.chest) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.stomach)) / (SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.head) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.chest) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.stomach) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.leftarm ) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.rightarm) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.leftleg) + SUM('
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.rightleg)) * 100, 1), 0.0) AS smmiddle
		FROM
			'
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '
		LEFT JOIN '
        . $xoopsDB->prefix('hlstats_Servers') . ' ON
			'
        . $xoopsDB->prefix('hlstats_Servers') . '.serverId='
        . $xoopsDB->prefix('hlstats_Events_Statsme2') . '.serverId
		WHERE
			'
        . $xoopsDB->prefix('hlstats_Servers') . ".game='$game' AND " . $xoopsDB->prefix('hlstats_Events_Statsme2') . ".PlayerId=$player
		GROUP BY
			" . $xoopsDB->prefix('hlstats_Events_Statsme2') . ".weapon
		ORDER BY
			$tblWeaponstats2->sort $tblWeaponstats2->sortorder,
			$tblWeaponstats2->sort2 $tblWeaponstats2->sortorder
	");	

if ($xoopsDB->getRowsNum($result) != 0)
{
?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="50%" colspan=2><a name="weaponstats2"></a>
<font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Weapon Target</b><?php echo '</font>';?></td>
	<td width="50%" align="right"><font class="fontNormal">(Last <?php echo DELETEDAYS; ?> Days)<?php echo '</font>';?></td>
</tr>

<tr>
	<td width="5%">&nbsp;</td>
	<td width="95%" colspan=2>&nbsp;<br>
	<?php
		$tblWeaponstats2->draw($result, $xoopsDB->getRowsNum($result), 100);
	?></td>
</tr>

</table><p>
<?php
}
	flush();
	
	$tblMaps = new Table(
        [
			new TableColumn(
                'map', 'Map Name',
                'width=25&align=center&link=' . urlencode("mode=mapinfo&map=%k&game=$game")
			),
			new TableColumn(
                'kpd', 'Kills per Death', 'width=10&align=right'
			),
			new TableColumn(
                'kills', 'Kills', 'width=10&align=right'
			),
			new TableColumn(
                'percentage', 'Percentage of Kills', 'width=30&sort=no&type=bargraph'
			),
			new TableColumn(
                'percentage', '%',
                'width=10&sort=no&align=right&append=' . urlencode('%')
			),
			new TableColumn(
                'deaths', 'Deaths', 'width=10&align=right'
			)
		], 'map', 'kpd', 'kills',
        true, 9999, 'maps_page', 'maps_sort', 'maps_sortorder', 'maps'
	);

	$result = $xoopsDB->query("
		SELECT
			IF(map='', '(Unaccounted)', map) AS map,
			SUM(killerId=$player) AS kills,
			SUM(victimId=$player) AS deaths,
			IFNULL(SUM(killerId=$player) / SUM(victimId=$player), '-') AS kpd,
			ROUND(CONCAT(SUM(killerId=$player)) / $realkills * 100, 2) AS percentage # workaround weird mysql bug
		FROM
			".$xoopsDB->prefix('hlstats_Events_Frags') . '
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . '.serverId=' . $xoopsDB->prefix('hlstats_Events_Frags') . '.serverId
		WHERE
			' . $xoopsDB->prefix('hlstats_Servers') . ".game='$game' AND killerId='$player'
			OR victimId='$player'
		GROUP BY
			map
		ORDER BY
			$tblMaps->sort $tblMaps->sortorder,
			$tblMaps->sort2 $tblMaps->sortorder
	");
?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="50%" colspan=2><a name="maps"></a>
<font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Map Performance</b><?php echo '</font>';?></td>
	<td width="50%" align="right"><font class="fontNormal">(Last <?php echo DELETEDAYS; ?> Days)<?php echo '</font>';?></td>
</tr>

<tr>
	<td width="5%">&nbsp;</td>
	<td width="95%" colspan=2>&nbsp;<br>
	<?php
		$tblMaps->draw($result, $xoopsDB->getRowsNum($result), 100);
	?></td>
</tr>

</table><p>

<?php
	flush();

	$tblPlayerKillStats = new Table(
        [
			new TableColumn(
                'name', 'Victim',
                'width=35&icon=player&link=' . urlencode('mode=playerinfo&player=%k')
			),
			new TableColumn(
                'kills', 'Times Killed', 'width=20&align=right'
			),
			new TableColumn(
                'deaths', 'Deaths by', 'width=20&align=right'
			),
			new TableColumn(
                'kpd', 'Kills per Death', 'width=20&align=right'
			)
		], 'victimId', 'kills', 'deaths', true,
        9999, 'playerkills_page', 'playerkills_sort', 'playerkills_sortorder', 'playerkills'
	);

	if(!isset($killLimit)) { $killLimit = 5; }

	//there might be a better way to do this, but I could not figure one out.

	 $xoopsDB->queryF('DROP TABLE IF EXISTS ' . $xoopsDB->prefix('hlstats_Frags_Kills') . '');
	 $xoopsDB->queryF(
         '
		CREATE TEMPORARY TABLE ' . $xoopsDB->prefix('hlstats_Frags_Kills') . '
		(
			playerId INT(10),
			kills INT(10),
			deaths INT(10)
		)
	'
     );
	 $xoopsDB->queryF(
         '
			INSERT INTO
				' . $xoopsDB->prefix('hlstats_Frags_Kills') . '
				(
					playerId,
					kills
				)
					SELECT
						victimId,
						killerId
					FROM
						' . $xoopsDB->prefix('hlstats_Events_Frags') . '
					LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
						' . $xoopsDB->prefix('hlstats_Servers') . '.serverId=' . $xoopsDB->prefix('hlstats_Events_Frags') . '.serverId
					WHERE
						' . $xoopsDB->prefix('hlstats_Servers') . ".game='$game' AND killerId = $player
	");

	 $xoopsDB->queryF(
         '
			INSERT INTO
				' . $xoopsDB->prefix('hlstats_Frags_Kills') . '
				(
					playerId,
					deaths
				)
					SELECT
						killerId,
						victimId
					FROM
						' . $xoopsDB->prefix('hlstats_Events_Frags') . '
					LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
						' . $xoopsDB->prefix('hlstats_Servers') . '.serverId=' . $xoopsDB->prefix('hlstats_Events_Frags') . '.serverId
					WHERE
						' . $xoopsDB->prefix('hlstats_Servers') . ".game='$game' AND victimId = $player
		");

		$result = $xoopsDB->query(
            '
				SELECT
					' . $xoopsDB->prefix('hlstats_Players') . '.lastName AS name,
					Count(' . $xoopsDB->prefix('hlstats_Frags_Kills') . '.kills) AS kills,
					Count(' . $xoopsDB->prefix('hlstats_Frags_Kills') . '.deaths) AS deaths,
					' . $xoopsDB->prefix('hlstats_Frags_Kills') . '.playerId as victimId,
					IFNULL(Count(' . $xoopsDB->prefix('hlstats_Frags_Kills') . '.kills)/Count(' . $xoopsDB->prefix('hlstats_Frags_Kills') . '.deaths),
						IFNULL(FORMAT(Count(' . $xoopsDB->prefix('hlstats_Frags_Kills') . ".kills), 2), '-')) AS kpd
				FROM
					" . $xoopsDB->prefix('hlstats_Frags_Kills') . '
				INNER JOIN
					' . $xoopsDB->prefix('hlstats_Players') . '
				ON
					' . $xoopsDB->prefix('hlstats_Frags_Kills') . '.playerId = ' . $xoopsDB->prefix('hlstats_Players') . '.playerId
				GROUP BY
					' . $xoopsDB->prefix('hlstats_Frags_Kills') . '.playerId
				HAVING
					Count(' . $xoopsDB->prefix('hlstats_Frags_Kills') . ".kills) >= $killLimit
				ORDER BY
	            $tblPlayerKillStats->sort $tblPlayerKillStats->sortorder,
	            $tblPlayerKillStats->sort2 $tblPlayerKillStats->sortorder
		");

	$numitems = $xoopsDB->getRowsNum($result);
			
  	if ($numitems > 0)
		{
?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>
<tr>
  <td width="50%" colspan=2><a name="playerkills"></a>
<font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Player Kill Statistics (<?php echo $killLimit ?> or more kills)</b><?php echo '</font>';?></td>
	<td width="50%" align="right"><font class="fontNormal">(Last <?php echo DELETEDAYS; ?> Days)<?php echo '</font>';?></td>
</tr>

<tr>
	<td width="5%">&nbsp;</td>
	<td width="95%" colspan=2>&nbsp;<br>
	<?php
    $tblPlayerKillStats->draw($result, $numitems, 100);
  ?></td>
</tr>
<tr>
	<td>&nbsp;</td>
	<td colspan=2><form method="GET" action="<?php echo $g_options['scripturl']; ?>">
	<font class="fontNormal">Show people this person has killed
	<SELECT name="killLimit" onchange='changeLimit(this.options[this.selectedIndex].value)'>
<?php
  for($j = 0; $j < 16; $j++) {
		echo "<option value=$j";
		if($killLimit == $j) { echo ' selected'; }
		echo ">$j</option>";
	}
?>
	</select>
	or more times in the last <?php echo DELETEDAYS; ?> days<?php echo '</font>';?>
	<script type="text/javascript" language="javascript">
	<!--
	function changeLimit(num) {
		location = "http://<?php echo $HTTP_HOST . $PHP_SELF ?>?mode=playerinfo&player=<?php echo $player ?>&killLimit=" + num + "#playerkills";
	}
	-->
	</script>
	</form></td>
</tr>
</table><p>
<?php
}
?>

<br>
<br>

<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="100%"><font class="fontNormal"><b>Note</b> Player event histories cover only the last <?php echo DELETEDAYS; ?> days. Items marked "Last <?php echo DELETEDAYS; ?> Days" or "*" above are generated from the player's Event History. Player kill, death and suicide totals and points ratings cover the entire recorded period.<?php echo '</font>';?></td>
</tr>
<?php
if(is_object($xoopsUser) && $xoopsUser->isAdmin()){
?>
<tr>
	<td width="100%" align="right"><br><br>
	<?php echo '<font class="fontSmall">'; ?><b>Admin Options:</b> <a href="<?= $g_options['admin_scripturl']?>?mode=admin&task=tools_editdetails_player&id=<?=$player?>">Edit Player Details</a><?php echo '</font>'; ?></td>
</tr>
<?php
}
?>
</table><p>
