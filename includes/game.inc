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
	 
	$sql = $xoopsDB->query('SELECT name FROM ' . $xoopsDB->prefix('hlstats_Games') . " WHERE code='$game'");
	if ($xoopsDB->getRowsNum($sql) < 1) error("No such game '$game'.");
	
	[$gamename] = $xoopsDB->fetchRow($sql);
	$xoopsDB->freeRecordSet($sql);

	pageHeader([$gamename], [$gamename =>'']);
?>

<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td><font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Sections</b><?php echo '</font>'; ?><p>
		
		<table width="75%" align="center" border=0 cellspacing=0 cellpadding=0>
		
		<tr valign="top">
			<td nowrap><font class="fontNormal"><a href="<?php echo $g_options['scripturl'] . "?mode=players&amp;game=$game"; ?>"><img src="<?php echo $g_options['imgdir']; ?>/player.gif" width=16 height=16 hspace=4 border=0 align="middle" alt="player.gif"><b>Player Rankings...</b></a><br>
		<img src="<?php echo $g_options['imgdir']; ?>/spacer.gif" width=1 height=5 border=0 alt="spacer.gif"><br>
		<a href="<?php echo $g_options['scripturl'] . "?mode=clans&amp;game=$game"; ?>"><img src="<?php echo $g_options['imgdir']; ?>/clan.gif" width=16 height=16 hspace=4 border=0 align="middle" alt="clan.gif"><b>Clan Rankings...</b></a><br>
		<img src="<?php echo $g_options['imgdir']; ?>/spacer.gif" width=1 height=5 border=0 alt=spacer.gif><br>
		<img src="<?php echo $g_options['imgdir']; ?>/spacer.gif" width=16 height=16 hspace=4 border=0 align="middle" alt="spacer.gif"><a href="<?php echo $g_options['scripturl'] . "?mode=weapons&amp;game=$game"; ?>"><b>Weapon Statistics...</b></a><br>
		<img src="<?php echo $g_options['imgdir']; ?>/spacer.gif" width=1 height=5 border=0 alt=spacer.gif><br>
                <img src="<?php echo $g_options['imgdir']; ?>/spacer.gif" width=16 height=16 hspace=4 border=0 align="middle" alt="spacer.gif"><a href="<?php echo $g_options['scripturl'] . "?mode=actions&amp;game=$game"; ?>"><b>Action Statistics...</b></a><br>
		<img src="<?php echo $g_options['imgdir']; ?>/spacer.gif" width=1 height=5 border=0 alt="spacer.gif"><br>
		<img src="<?php echo $g_options['imgdir']; ?>/spacer.gif" width=16 height=16 hspace=4 border=0 align="middle" alt="spacer.gif"><a href="<?php echo $g_options['scripturl'] . "?mode=maps&amp;game=$game"; ?>"><b>Map Statistics...</b></a></font></td>
<?php if (defined('SHOW_STATSDISCLAIMER')) { ?>
			<td width="50%">
				<table width="100%" border=0 cellspacing=0 cellpadding=5>
				
				<tr>
					<td><font class="fontNormal"><b>NOTICE</b><br>
<img src="<?php echo $g_options['imgdir']; ?>/spacer.gif" width=1 height=5 border=0><br>
These rankings and statistics are intended to present only a very simplistic measure of player performance.<br>
<img src="<?php echo $g_options['imgdir']; ?>/spacer.gif" width=1 height=5 border=0><br>
These statistics should not be taken as a realistic measure of individual player skill.<br>

</font></td>
				</tr>
				
				</table></td>
<?php } ?>
		</tr>
		
		</table></td>
</tr>

</table><p>
<br>

<?php
	$resultAwards = $xoopsDB->query('
		SELECT
			' . $xoopsDB->prefix('hlstats_Awards') . '.awardType,
			' . $xoopsDB->prefix('hlstats_Awards') . '.code,
			' . $xoopsDB->prefix('hlstats_Awards') . '.name,
			' . $xoopsDB->prefix('hlstats_Awards') . '.verb,
			' . $xoopsDB->prefix('hlstats_Awards') . '.d_winner_id,
			' . $xoopsDB->prefix('hlstats_Awards') . '.d_winner_count,
			' . $xoopsDB->prefix('hlstats_Players') . '.lastName AS d_winner_name
		FROM
			' . $xoopsDB->prefix('hlstats_Awards') . '
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' ON
			' . $xoopsDB->prefix('hlstats_Players') . '.playerId = ' . $xoopsDB->prefix('hlstats_Awards') . '.d_winner_id
		WHERE
			' . $xoopsDB->prefix('hlstats_Awards') . ".game='$game'
		ORDER BY
			".$xoopsDB->prefix('hlstats_Awards').'.awardType DESC,
			' . $xoopsDB->prefix('hlstats_Awards') . '.name ASC
	'
    );

$result = $xoopsDB->query('
		SELECT
			IFNULL(value, 1)
		FROM
			' . $xoopsDB->prefix('hlstats_Options') . "
		WHERE
			keyname='awards_numdays'
	");

	if ($xoopsDB->getRowsNum($result) == 1)
		[$awards_numdays] = $xoopsDB->fetchRow($result);
	else
		$awards_numdays = 1;

	$result = $xoopsDB->query("
		SELECT
			DATE_FORMAT(value, '%W %e %b'),
			DATE_FORMAT( DATE_SUB( value, INTERVAL $awards_numdays DAY ) , '%W %e %b' )
		FROM
			".$xoopsDB->prefix('hlstats_Options')."
		WHERE
			keyname='awards_d_date'
	");
	[$awards_d_date, $awards_s_date] = $xoopsDB->fetchRow($result);

	if ($xoopsDB->getRowsNum($resultAwards) > 0 && $awards_d_date)
	{
?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td><font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp; <?php if ($awards_numdays == 1) {echo 'Daily';} else {
                    echo "$awards_numdays Day";}; ?> Awards (<?php echo "$awards_s_date to $awards_d_date."; ?>)</b><?php echo '</font>';?><p>

    <table width="75%" align="center" border=0 cellspacing=0 cellpadding=0>
		<tr>
			<td>
				<table width="100%" border=0 cellspacing=1 cellpadding=4>

<?php
		$c = 0;
		while (false !== ($awarddata = $xoopsDB->fetchArray($resultAwards)))
		{
			$colour = ($c % 2) + 1;
			$c++;
?>

<tr>
	<td width="30%"><?php
			echo '<font class="fontNormal">';
        echo htmlspecialchars($awarddata['name'], ENT_QUOTES | ENT_HTML5);
        echo '</font>';
        ?></td>
    <td width="70%"><?php
			echo '<font class="fontNormal">';

        if ($awarddata['d_winner_id']) {
            echo '<a href="' . $g_options['scripturl']
                 . '?mode=playerinfo&amp;player='
                 . $awarddata['d_winner_id']
                 . '"><img src="'
                 . $g_options['imgdir'] . '/player.gif" width=16 height=16 '
                 . 'hspace=4 border=0 align="middle" alt="player.gif"><b>'
                 . htmlspecialchars($awarddata['d_winner_name'], ENT_QUOTES | ENT_HTML5)
                 . '</b></a> ('
                 . $awarddata['d_winner_count']
                 . ' '
                 . htmlspecialchars($awarddata['verb'], ENT_QUOTES | ENT_HTML5) . ')';
        } else
			{
				echo '&nbsp;&nbsp;(Nobody)';
            }

        echo '</font>';
        ?></td>
</tr>

<?php
		}
?></table></td>
		</tr>
		
		</table></td>
</tr>

</table><p>
<br>

<?php
	}
?>

<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td><font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Participating Servers</b><?php echo '</font>';?><p>

<table width="75%" align="center" border=0 cellspacing=0 cellpadding=0>
		
		<tr>
			<td><table width="100%" border=0 cellspacing=1 cellpadding=4>
			
				<tr valign="bottom">
					<td width="60%"><?php echo '<font class="fontSmall">'; ?><font>&nbsp;Name</font><?php echo '</font>'; ?></td>
                    <td width="40%"><?php echo '<font class="fontSmall">'; ?><font>&nbsp;Address</font><?php echo '</font>'; ?></td>
                    <td width="20%"><?php echo '<font class="fontSmall">'; ?><font>&nbsp;Statistics</font><?php echo '</font>'; ?></td>
                </tr>
		
<?php
		$sql2 = $xoopsDB->query("
			SELECT
				serverId,
				name,
				IF(publicaddress != '',
					publicaddress,
					concat(address, ':', port)
				) AS addr,
				statusurl
			FROM
				".$xoopsDB->prefix('hlstats_Servers')."
			WHERE
				game='$game'
			ORDER BY
				name ASC,
				addr ASC
		");
		
		$i=0;
		while (false !== ($rowdata = $xoopsDB->fetchArray($sql2)))
		{
			$c = ($i % 2) + 1;
			
			if ($rowdata['statusurl']) {
                $addr = '<a href="' . $rowdata['statusurl'] . '">' . $rowdata['addr'] . '</a>';
            } else
			{
				$addr = $rowdata['addr'];
            }
            ?>

            <tr valign="middle">
					<td align="left"><font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/server.gif" width=16 height=16 hspace=3 border=0 align="middle" alt="server.gif"><?php
						echo $rowdata['name'];
                        echo '</font>'; ?></td>
                <td align="left"><?php
						echo '<font class="fontNormal">';
                    echo $addr;
						echo '</font>';
                    ?></td>
					<td align="center"><?php
						echo '<font class="fontNormal">';
                        echo "<a href=\"$g_options[scripturl]?mode=live_stats&amp;server=$rowdata[serverId]\">View</a>";
						echo '</font>';
                        ?></td>
				</tr>
<?php $i++;
		}
?>
		
				</table></td>
		</tr>
		
		</table></td>
</tr>

</table><p>
<br>

<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td><font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;<?php echo $gamename; ?> Statistics</b><?php echo '</font>';?><p>

    <?php
			$result = $xoopsDB->query('SELECT COUNT(*) FROM ' . $xoopsDB->prefix('hlstats_Players') . " WHERE game='$game'");
			[$num_players] = $xoopsDB->fetchRow($result);
			$num_players = (int)$num_players;
			
			$result = $xoopsDB->query('SELECT COUNT(*) FROM ' . $xoopsDB->prefix('hlstats_Clans') . " WHERE game='$game'");
			[$num_clans] = $xoopsDB->fetchRow($result);
			$num_clans = (int)$num_clans;
			
			$result = $xoopsDB->query('SELECT COUNT(*) FROM ' . $xoopsDB->prefix('hlstats_Servers') . " WHERE game='$game'");
			[$num_servers] = $xoopsDB->fetchRow($result);
			$num_servers = (int)$num_servers;
			
			$result = $xoopsDB->query("
				SELECT
					DATE_FORMAT(MAX(eventTime), '%r, %a. %e %b.')
				FROM
					".$xoopsDB->prefix('hlstats_Events_Frags').'
				LEFT JOIN '.$xoopsDB->prefix('hlstats_Players') . ' ON
					'.$xoopsDB->prefix('hlstats_Players') . '.playerId = ' . $xoopsDB->prefix('hlstats_Events_Frags') . '.serverId
				WHERE
					' . $xoopsDB->prefix('hlstats_Players') . ".game='$game'
			");
			[$lastevent] = $xoopsDB->fetchRow($result);
?>

		<table width="75%" align="center" border=0 cellspacing=0 cellpadding=3>
		
		<tr valign="top">
			<td width=10><font class="fontNormal"><b>&#149;&nbsp;</b></font></td>
			<td width="100%"><?php
				echo '<font class="fontNormal">';

                echo "<b>$num_players</b> players and <b>$num_clans</b> clans "
					. "ranked on <b>$num_servers</b> servers.";
					
				echo '</font>';
			?></td>
		</tr>

            <?php
            if ($lastevent)
			{
?>
		<tr valign="top">
			<td width=10><font class="fontNormal"><b>&#149;&nbsp;</b></font></td>
			<td width="100%"><?php
				echo '<font class="fontNormal">';

                echo "Last kill <b>$lastevent</b>";
				
				echo '</font>';
			?></td>
		</tr>
                <?php
            }
?>
		
		<tr valign="top">
			<td width=10><font class="fontNormal"><b>&#149;&nbsp;</b></font></td>
			<td width="100%"><?php
				echo '<font class="fontNormal">';

                echo 'All statistics are generated in real-time. Event history data expires after <b>' . DELETEDAYS . '</b> days.';
					
				echo '</font>';
                ?></td>
		</tr>

        </table></td>
</tr>

</table><p>
<br>
