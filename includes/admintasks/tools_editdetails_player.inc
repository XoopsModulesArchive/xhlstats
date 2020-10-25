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
	 
	if ($auth->userdata['acclevel'] < 80) die ('Access denied!');
?>

&nbsp;&nbsp;&nbsp;&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;<a href="<?php echo $g_options['admin_scripturl']; ?>?mode=admin&task=tools_editdetails">Edit Player or Clan Details</a></b><br>

<img src="<?php echo $g_options['imgdir']; ?>/spacer.gif" width=1 height=8 border=0><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;<?php echo "Edit Player #$id"; ?></b><p>

<form method="POST" action="<?php echo $g_options['admin_scripturl'] . "?mode=admin&task=$selTask&id=$id"; ?>">
<?php
	$clanresult = $xoopsDB->query(
        '
		SELECT
			clanId, tag
		FROM
			' . $xoopsDB->prefix('hlstats_Clans') . '
		ORDER BY
			tag
		ASC
	'
    );
	$clanList = '';
	while(list($clanId,$tag) = $xoopsDB->fetchRow($clanresult)){
		$clanList .= ';'.$clanId.'/'.$tag;
	}

	$proppage = new PropertyPage(
        '' . $xoopsDB->prefix('hlstats_Players') . '', 'playerId', $id, [
		new PropertyPage_Group(
            'Profile', [
                         new PropertyPage_Property('fullName', 'Real Name', 'text'),
                         new PropertyPage_Property('email', 'E-mail Address', 'text'),
                         new PropertyPage_Property('homepage', 'Homepage URL', 'text'),
                         new PropertyPage_Property('icq', 'ICQ Number', 'text'),
                         new PropertyPage_Property('clan', 'Associated with Clan', 'select', "0/None$clanList"),
                         new PropertyPage_Property('hideranking', 'Hide Ranking', 'select', '0/No;1/Yes'),
        ]
        )
    ]
    );
	
	
	if ($_POST)
	{
		$proppage->update();
		message('success', 'Profile updated successfully.');
	}
	
	
	
	$result = $xoopsDB->query(
        '
		SELECT
			*
		FROM
			' . $xoopsDB->prefix('hlstats_Players') . "
		WHERE
			playerId='$id'
	");
	if ($xoopsDB->getRowsNum($result) < 1) die("No player exists with ID #$id");
	
	$data = $xoopsDB->fetchArray($result);
	
	echo '<font>' . $data['lastName'] . '</font>';
	
	echo '<font>';
	echo '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
         . '<a href="' . $g_options['scripturl'] . "?mode=playerinfo&player=$id" . '">'
         . '(View Player Details)</a>';
	echo '</font>';
?><p>

<table width="60%" align="center" border=0 cellspacing=0 cellpadding=0>
<tr>
	<td><?php
		echo '<font>';
		$proppage->draw($data);
		echo '</font>';
?>
	<center><input type="submit" value="  Apply  " class="submit"></center></td>
</tr>
</table>
</form>

