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

	$edlist = new EditList('id', '' . $xoopsDB->prefix('hlstats_ClanTags') . '', 'clan', true);
	$edlist->columns[] = new EditListColumn('pattern', 'Pattern', 40, true, 'text', '', 64);
	$edlist->columns[] = new EditListColumn('position', 'Match Position', 0, true, 'select', 'EITHER/EITHER;START/START only;END/END only');
	
	
	if ($_POST)
	{
		if ($edlist->update())
			message('success', 'Operation successful.');
		else
			message('warning', $edlist->error());
	}
	
?>

Here you can define the patterns used to determine what clan a player is in. These patterns are applied to players' names when they connect or change name.<p>

Special characters in the pattern:<p>

<table border=0 cellspacing=0 cellpadding=4>

<tr>
	<td><?php echo '<font class="fontSmall">'; ?>Character<?php echo '</font>'; ?></td>
	<td><?php echo '<font class="fontSmall">'; ?>Description<?php echo '</font>'; ?></td>
</tr>

<tr>
	<td><font class="fontNormal"><tt>A</tt></font></td>
	<td><font class="fontNormal">Matches one character  (i.e. a character is required)</font></td>
</tr>

<tr>
	<td><font class="fontNormal"><tt>X</tt></font></td>
	<td><font class="fontNormal">Matches zero or one characters  (i.e. a character is optional)</font></td>
</tr>

<tr>
	<td><font class="fontNormal"><tt>a</tt></font></td>
	<td><font class="fontNormal">Matches literal A or a</font></td>
</tr>

<tr>
	<td><font class="fontNormal"><tt>x</tt></font></td>
	<td><font class="fontNormal">Matches literal X or x</font></td>
</tr>

</table><p>

Example patterns:<p>

<table border=0 cellspacing=0 cellpadding=4>

<tr>
	<td><?php echo '<font class="fontSmall">'; ?>Pattern<?php echo '</font>'; ?></td>
	<td><?php echo '<font class="fontSmall">'; ?>Description<?php echo '</font>'; ?></td>
	<td><?php echo '<font class="fontSmall">'; ?>Example<?php echo '</font>'; ?></td>
</tr>

<tr>
	<td><font class="fontNormal"><tt>[AXXXXX]</tt></font></td>
	<td><font class="fontNormal">Matches 1 to 6 characters inside square braces</font></td>
	<td><font class="fontNormal"><tt>[ZOOM]Player</tt></font></td>
</tr>

<tr>
	<td><font class="fontNormal"><tt>{AAXX}</tt></font></td>
	<td><font class="fontNormal">Matches 2 to 4 characters inside curly braces</font></td>
	<td><font class="fontNormal"><tt>{S3G}Player</tt></font></td>
</tr>

<tr>
	<td><font class="fontNormal"><tt>rex>></tt></font></td>
	<td><font class="fontNormal">Matches the string "rex>>", "REX>>", etc.</font></td>
	<td><font class="fontNormal"><tt>REX>>Tyranno</tt></font></td>
</tr>

</table><p>

Avoid adding patterns to the database that are too generic. Always ensure you have at least one literal (non-special) character in the pattern -- for example if you were to add the pattern "AXXA", it would match any player with 2 or more letters in their name!<p>

The Match Position field sets which end of the player's name the clan tag is allowed to appear.<p>

<?php
	
	$result = $xoopsDB->query(
        '
		SELECT
			id,
			pattern,
			position
		FROM
			' . $xoopsDB->prefix('hlstats_ClanTags') . '
		ORDER BY
			position,
			pattern,
			id
	'
    );
	
	$edlist->draw($result);
?>

<table width="75%" border=0 cellspacing=0 cellpadding=0>
<tr>
	<td align="center"><input type="submit" value="  Apply  " class="submit"></td>
</tr>
</table>

