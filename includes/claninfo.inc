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
	
	
	// Clan Details
	
	$clan = (int)$_GET["clan"]
		or error('No clan ID specified.');
	
	$sql = $xoopsDB->query(
        '
		SELECT
			' . $xoopsDB->prefix('hlstats_Clans') . '.tag,
			' . $xoopsDB->prefix('hlstats_Clans') . '.name,
			' . $xoopsDB->prefix('hlstats_Clans') . '.homepage,
			' . $xoopsDB->prefix('hlstats_Clans') . '.game,
			SUM(' . $xoopsDB->prefix('hlstats_Players') . '.kills) AS kills,
			SUM(' . $xoopsDB->prefix('hlstats_Players') . '.deaths) AS deaths,
			COUNT(' . $xoopsDB->prefix('hlstats_Players') . '.playerId) AS nummembers,
			ROUND(AVG(' . $xoopsDB->prefix('hlstats_Players') . '.skill)) AS avgskill
		FROM
			' . $xoopsDB->prefix('hlstats_Clans') . '
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' ON
			' . $xoopsDB->prefix('hlstats_Players') . '.clan = ' . $xoopsDB->prefix('hlstats_Clans') . '.clanId
		WHERE
			' . $xoopsDB->prefix('hlstats_Clans') . ".clanId=$clan
		GROUP BY
			" . $xoopsDB->prefix('hlstats_Clans') . '.clanId
	'
    );
	if ($xoopsDB->getRowsNum($sql) != 1)
		error("No such clan '$clan'.");
	
	$clandata = $xoopsDB->fetchArray($sql);
	$xoopsDB->freeRecordSet($sql);
	
	
	$cl_name = ereg_replace(' ', '&nbsp;', htmlspecialchars($clandata['name'], ENT_QUOTES | ENT_HTML5));
	$cl_tag  = ereg_replace(' ', '&nbsp;', htmlspecialchars($clandata['tag'], ENT_QUOTES | ENT_HTML5));
	$cl_full = $cl_tag . ' ' . $cl_name;
	
	$game = $clandata['game'];
	$sql2 = $xoopsDB->query('SELECT name FROM ' . $xoopsDB->prefix('hlstats_Games') . " WHERE code='$game'");
	if ($xoopsDB->getRowsNum($sql2) != 1)
		$gamename = ucfirst($game);
	else
		[$gamename] = $xoopsDB->fetchRow($sql2);
	
	
	pageHeader(
        [$gamename, 'Clan Details', $cl_full],
        [
            $gamename       => $g_options['scripturl'] . "?game=$game",
            'Clan Rankings' => $g_options['scripturl'] . "?mode=clans&amp;game=$game",
            'Clan Details'  => ''
		],
        $clandata['name']
	);
?>



<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="100%" colspan=2><font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Clan Profile and Statistics Summary</b><?php echo '</font>';?></td>
</tr>

<tr valign="top">
	<td width="5%">&nbsp;</td>
	<td width="95%">&nbsp;<br>
		<table width="60%" border=0 cellspacing=0 cellpadding=0>
		<tr>
			<td>
				<table width="100%" border=0 cellspacing=1 cellpadding=4>

				<tr>
					<td><?php
						echo '<font class="fontNormal">';
						echo 'Home Page:';
						echo '</font>';
					?></td>
					<td><?php
						echo '<font class="fontNormal">';
						if ($url = getLink($clandata['homepage']))
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
						echo 'Number of Members:';
						echo '</font>';
					?></td>
					<td><?php
						echo '<font class="fontNormal">';
						echo $clandata['nummembers'];
						echo '</font>';
					?></td>
				</tr>
				
				<tr>
					<td><?php
						echo '<font class="fontNormal">';
						echo 'Avg. Member Points:';
						echo '</font>';
					?></td>
					<td><?php
						echo '<font class="fontNormal">';
						echo $clandata['avgskill'];
						echo '</font>';
					?></td>
				</tr>
				
				<tr>
					<td><?php
						echo '<font class="fontNormal">';
						echo 'Total Kills:';
						echo '</font>';
					?></td>
					<td><?php
						echo '<font class="fontNormal">';
						echo $clandata['kills'];
						echo '</font>';
					?></td>
				</tr>
				
				<tr>
					<td><?php
						echo '<font class="fontNormal">';
						echo 'Total Deaths:';
						echo '</font>';
					?></td>
					<td><?php
						echo '<font class="fontNormal">';
						echo $clandata['deaths'];
						echo '</font>';
					?></td>
				</tr>
				
				<tr>
					<td><?php
						echo '<font class="fontNormal">';
						echo 'Kills per Death:';
						echo '</font>';
					?></td>
					<td><?php
						echo '<font class="fontNormal">';
						if ($clandata['deaths'] != 0)
						{
							printf('%0.2f', $clandata['kills'] / $clandata['deaths']);
						}
						else
						{
							echo '-';
						}
						echo '</font>';
					?></td>
				</tr>


				</table></td>
		</tr>
		
		</table></td>
</tr>

</table><p>

<?php
	flush();
	
	$tblMembers = new Table(
        [
            new TableColumn(
                'lastName', 'Name',
                'width=30&icon=player&link=' . urlencode('mode=playerinfo&amp;player=%k')
			),
            new TableColumn(
                'skill', 'Points', 'width=8&align=right'
			),
            new TableColumn(
                'kills', 'Kills', 'width=8&align=right'
			),
            new TableColumn(
                'percent', 'Contribution&nbsp;to Clan&nbsp;Kills', 'width=20&sort=no&type=bargraph'
			),
            new TableColumn(
                'percent', '%',
                'width=8&sort=no&align=right&append=' . urlencode('%')
			),
            new TableColumn(
                'deaths', 'Deaths', 'width=8&align=right'
			),
            new TableColumn(
                'kpd', 'Kills per Death', 'width=8&align=right'
			),
            new TableColumn(
                'playerId', 'ID', 'width=5&align=right&sort=no'
			)
		], 'playerId', 'skill', 'kpd',
        true,
        20, 'members_page', 'members_sort', 'members_sortorder', 'members'
	);

	$result = $xoopsDB->query("
		SELECT
			playerId,
			lastName,
			skill,
			kills,
			deaths,
			IFNULL(kills/deaths, '-') AS kpd,
			(kills/" . $clandata['kills'] . ') * 100 AS percent
		FROM
			' . $xoopsDB->prefix('hlstats_Players') . "
		WHERE
			clan=$clan
		ORDER BY
			$tblMembers->sort $tblMembers->sortorder,
			$tblMembers->sort2 $tblMembers->sortorder,
			lastName ASC
		LIMIT $tblMembers->startitem,$tblMembers->numperpage
	");
	
	$resultCount = $xoopsDB->query(
        '
		SELECT
			COUNT(*)
		FROM
			' . $xoopsDB->prefix('hlstats_Players') . "
		WHERE
			clan=$clan
	");
	
	[$numitems] = $xoopsDB->fetchRow($resultCount);
?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="100%" colspan=2><a name="members"></a>
<font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Members</b><?php echo '</font>';?></td>
</tr>

<tr>
	<td width="5%">&nbsp;</td>
	<td width="95%">&nbsp;<br>
	<?php
		$tblMembers->draw($result, $numitems, 100);
	?></td>
</tr>

</table><p>

<br>
<br>

<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>
<?php
if(is_object($xoopsUser) && $xoopsUser->isAdmin()){
?>
<tr>
	<td width="100%" align="right"><br><br>
	<?php echo '<font class="fontSmall">'; ?><b>Admin Options:</b> <a href="<?= $g_options['admin_scripturl']?>?mode=admin&amp;task=tools_editdetails_clan&amp;id=<?=$clan?>">Edit Clan Details</a><?php echo '</font>'; ?></td>
</tr>
<?php
}
?>
</table><p>
