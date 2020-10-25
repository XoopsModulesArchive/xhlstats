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
	
	// Map Details
	
	$map = $_GET['map']
		or error('No map specified.');
	
	$sql = $xoopsDB->query('SELECT name FROM ' . $xoopsDB->prefix('hlstats_Games') . " WHERE code='$game'");
	if ($xoopsDB->getRowsNum($sql) != 1)
		error('Invalid or no game specified.');
	else
		[$gamename] = $xoopsDB->fetchRow($sql);
		
	pageHeader(
        [$gamename, 'Map Details', $map],
        [
            $gamename        => $g_options['scripturl'] . "?game=$game",
            'Map Statistics' => $g_options['scripturl'] . "?mode=maps&amp;game=$game",
            'Map Details'    => ''
		],
        $map
	);



	$table = new Table(
        [
            new TableColumn(
                'killerName', 'Player',
                'width=60&align=left&icon=player&link=' . urlencode('mode=playerinfo&amp;player=%k')
			),
            new TableColumn(
                'frags',
                "Kills on $map", 'width=35&align=right'
			)
		], 'killerId', // keycol
        'frags', // sort_default
        'killerName', // sort_default2
        true, // showranking
        50 // numperpage
	);
	
	$result = $xoopsDB->query(
        '
		SELECT
			' . $xoopsDB->prefix('hlstats_Events_Frags') . '.killerId,
			' . $xoopsDB->prefix('hlstats_Players') . '.lastName AS killerName,
			COUNT(' . $xoopsDB->prefix('hlstats_Events_Frags') . '.map) AS frags
		FROM
			' . $xoopsDB->prefix('hlstats_Events_Frags') . '
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' ON
			' . $xoopsDB->prefix('hlstats_Players') . '.playerId = ' . $xoopsDB->prefix('hlstats_Events_Frags') . '.killerId
		WHERE
			' . $xoopsDB->prefix('hlstats_Events_Frags') . ".map='$map'
			AND " . $xoopsDB->prefix('hlstats_Players') . ".game='$game'
			AND " . $xoopsDB->prefix('hlstats_Players') . ".hideranking<>'1'
		GROUP BY
			" . $xoopsDB->prefix('hlstats_Events_Frags') . ".killerId
		ORDER BY
			$table->sort $table->sortorder,
			$table->sort2 $table->sortorder
		LIMIT $table->startitem,$table->numperpage
	");
	
	$resultCount = $xoopsDB->query(
        '
		SELECT
			COUNT(DISTINCT ' . $xoopsDB->prefix('hlstats_Events_Frags') . '.killerId),
			SUM(' . $xoopsDB->prefix('hlstats_Events_Frags') . ".map='$map')
		FROM
			" . $xoopsDB->prefix('hlstats_Events_Frags') . '
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' ON
			' . $xoopsDB->prefix('hlstats_Players') . '.playerId = ' . $xoopsDB->prefix('hlstats_Events_Frags') . '.killerId
		WHERE
			' . $xoopsDB->prefix('hlstats_Events_Frags') . ".map='$map'
			AND " . $xoopsDB->prefix('hlstats_Players') . ".game='$game'
	");
	
	[$numitems, $totalkills] = $xoopsDB->fetchRow($resultCount);
?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="50%"><font class="fontNormal">From a total of <b><?php echo (int)$totalkills; ?></b> kills (Last <?php echo DELETEDAYS; ?> Days)</font></td>
	<td width="50%" align="right"><font class="fontNormal">Back to <a href="<?php echo $g_options['scripturl'] . "?mode=maps&amp;game=$game"; ?>">Map Statistics</a></font></td>
</tr>

</table><p>
<?php // figure out URL and absolute path of image
	$imgurl = $g_options['imgdir'] . "/maps/$game/"
              . (string)$map;
	
	$image = getImage("/maps/$game/$map");
	
	if ($image || $g_options['map_dlurl'])
	{
?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr valign="top">
	<td width="90%"><?php
		$table->draw($result, $numitems, 100, 'center');
?></td>
	<td width=10>&nbsp;&nbsp;&nbsp;&nbsp;</td>
	<td width="10%" align="right" nowrap><?php
		echo '<font class="fontNormal">';
		
		if ($image)
		{
			echo '<img src="' . $image['url'] . '" ' . $image['size'] . " border=1 alt=\"$map\">";
		}
		if ($g_options['map_dlurl'])
		{
			$map_dlurl = str_replace('%MAP%', $map, $g_options['map_dlurl']);
			$map_dlurl = str_replace('%GAME%', $game, $map_dlurl);
			echo "<p><a href=\"$map_dlurl\">Download this map...</a></p>";
		}
		
		echo '</font>';
?></td>
</tr>

</table>
<?php
	}
	else
	{
		$table->draw($result, $numitems, 70, 'center');
	}
?>
