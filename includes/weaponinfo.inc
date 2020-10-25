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
	
	// Weapon Details
	
	$weapon = $_GET['weapon']
		or error('No weapon ID specified.');
	
	$sql = $xoopsDB->query(
        '
		SELECT
			name
		FROM
			' . $xoopsDB->prefix('hlstats_Weapons') . "
		WHERE
			code='$weapon'
			AND game='$game'
	");
	
	if ($xoopsDB->getRowsNum($sql) != 1)
	{
		$wep_name = ucfirst($weapon);
	}
	else
	{
		$weapondata = $xoopsDB->fetchArray($sql);
		$xoopsDB->freeRecordSet($sql);
		$wep_name = $weapondata['name'];
	}
	
	$sql1 = $xoopsDB->query('SELECT name FROM ' . $xoopsDB->prefix('hlstats_Games') . " WHERE code='$game'");
	if ($xoopsDB->getRowsNum($sql1) != 1)
		error('Invalid or no game specified.');
	else
		[$gamename] = $xoopsDB->fetchRow($sql1);
		
	pageHeader(
        [$gamename, 'Weapon Details', htmlspecialchars($wep_name, ENT_QUOTES | ENT_HTML5)],
        [
            $gamename           => $g_options['scripturl'] . "?game=$game",
            'Weapon Statistics' => $g_options['scripturl'] . "?mode=weapons&amp;game=$game",
            'Weapon Details'    => ''
		],
        $wep_name
	);



	$table = new Table(
        [
            new TableColumn(
                'killerName', 'Player',
                'width=60&align=left&icon=player&link=' . urlencode('mode=playerinfo&amp;player=%k')
			),
            new TableColumn(
                'frags',
                ucfirst($weapon) . ' kills', 'width=35&align=right'
			),
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
			COUNT(' . $xoopsDB->prefix('hlstats_Events_Frags') . '.weapon) AS frags
		FROM
			' . $xoopsDB->prefix('hlstats_Events_Frags') . '
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' ON
			' . $xoopsDB->prefix('hlstats_Players') . '.playerId = ' . $xoopsDB->prefix('hlstats_Events_Frags') . '.killerId
		WHERE
			' . $xoopsDB->prefix('hlstats_Events_Frags') . ".weapon='$weapon'
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
			SUM(' . $xoopsDB->prefix('hlstats_Events_Frags') . ".weapon='$weapon')
		FROM
			" . $xoopsDB->prefix('hlstats_Events_Frags') . '
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' ON
			' . $xoopsDB->prefix('hlstats_Players') . '.playerId = ' . $xoopsDB->prefix('hlstats_Events_Frags') . '.killerId
		WHERE
			' . $xoopsDB->prefix('hlstats_Events_Frags') . ".weapon='$weapon'
			AND " . $xoopsDB->prefix('hlstats_Players') . ".game='$game'
	");
	
	[$numitems, $totalkills] = $xoopsDB->fetchRow($resultCount);
?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="50%"><font class="fontNormal">From a total of <b><?php echo (int)$totalkills; ?></b> kills (Last <?php echo DELETEDAYS; ?> Days)</font></td>
	<td width="50%" align="right"><font class="fontNormal">Back to <a href="<?php echo $g_options['scripturl'] . "?mode=weapons&amp;game=$game"; ?>">Weapon Statistics</a></font></td>
</tr>

</table><p>
<?php $table->draw($result, $numitems, 70, 'center');
?>
