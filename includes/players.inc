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
	
	
	
	// Player Rankings
	
	$sql = $xoopsDB->query('SELECT name FROM ' . $xoopsDB->prefix('hlstats_Games') . " WHERE code='$game'");
	if ($xoopsDB->getRowsNum($sql) < 1) error("No such game '$game'.");
	
	[$gamename] = $xoopsDB->fetchRow($sql);
	$xoopsDB->freeRecordSet($sql);
	
	if (isset($_GET['minkills']))
	{
		$minkills = (int)$_GET["minkills"];
	}
	else
	{
		$minkills = 1;
	}
	
	pageHeader(
        [$gamename, 'Player Rankings'],
        [$gamename =>"%s?game=$game", 'Player Rankings' => '']
	);
?>

<form method="GET" action="<?php echo $g_options['scripturl']; ?>">
<input type="hidden" name="mode" value="search">
<input type="hidden" name="game" value="<?php echo $game; ?>">
<input type="hidden" name="st" value="player">

<table width="90%" align="center" border=0 cellspacing=0 cellpadding=2>

<tr valign="bottom">
	<td width="75%"><font class="fontNormal"><b>&#149;</b> Find a player: <input type="text" name="q" size=20 maxlength=64 class="textbox"> <input type="submit" value="Search" class="smallsubmit"></font></td>
	<td width="25%" align="right" nowrap><font class="fontNormal">Go to <a href="<?php echo $g_options['scripturl'] . "?mode=clans&amp;game=$game"; ?>"><img src="<?php echo $g_options['imgdir']; ?>/clan.gif" width=16 height=16 hspace=3 border=0 align="middle" alt="clan.gif">Clan Rankings</a></font></td>
</tr>

</table><p>
</form>

<?php
	$table = new Table(
        [
            new TableColumn(
                'lastName', 'Name',
                'width=46&icon=player&link=' . urlencode('mode=playerinfo&amp;player=%k')
			),
            new TableColumn(
                'skill', 'Points', 'width=11&align=right'
			),
            new TableColumn(
                'kills', 'Kills', 'width=11&align=right'
			),
            new TableColumn(
                'deaths', 'Deaths', 'width=11&align=right'
			),
            new TableColumn(
                'kpd', 'Kills per Death', 'width=11&align=right'
			),
            new TableColumn(
                'playerId', 'ID', 'width=5&align=right&sort=no'
			)
		], 'playerId', 'skill', 'kpd',
        true
	);
	
	$result = $xoopsDB->query("
		SELECT
			playerId,
			lastName,
			skill,
			kills,
			deaths,
			IFNULL(kills/deaths, '-') AS kpd
		FROM
			".$xoopsDB->prefix('hlstats_Players')."
		WHERE
			game='$game'
			AND hideranking=0
			AND kills >= $minkills
		ORDER BY
			$table->sort $table->sortorder,
			$table->sort2 $table->sortorder,
			lastName ASC
		LIMIT $table->startitem,$table->numperpage
	");
	
	$resultCount = $xoopsDB->query(
        '
		SELECT
			COUNT(*)
		FROM
			' . $xoopsDB->prefix('hlstats_Players') . "
		WHERE
			game='$game'
			AND hideranking=0
			AND kills >= $minkills
	");
	
	[$numitems] = $xoopsDB->fetchRow($resultCount);
	
	$table->draw($result, $numitems, 90);
?><p>

<form method="GET" action="<?php echo $g_options['scripturl']; ?>">
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=2>

<tr valign="bottom">
	<td width="75%"><font class="fontNormal">
<?php
	foreach ($_GET as $k=>$v)
	{
		if ($k != 'minkills')
		{
			echo "		<input type=\"hidden\" name=\"$k\" value=\"" . htmlspecialchars($v, ENT_QUOTES | ENT_HTML5) . "\">\n";
		}
	}
?>
		<b>&#149;</b> Only show players with <input type="text" name="minkills" size=4 maxlength=2 value="<?php echo $minkills; ?>" class="textbox"> or more kills. <input type="submit" value="Apply" class="smallsubmit"></font> </td>
</tr>

</table>
</form>
