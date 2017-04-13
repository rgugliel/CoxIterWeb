<?php
require('config/config.php');
?>
<!DOCTYPE HTML>
<html>

<head>
	<title>CoxIterWeb</title>
	<meta name="description" content="website description" />
	<meta name="keywords" content="website keywords, website keywords" />
	<meta http-equiv="content-type" content="text/html;charset=utf-8" />
	<link rel="stylesheet" type="text/css" href="style/style.css" />
	<link rel="stylesheet" type="text/css" href="resources/popup.css" />
	
	<link href="resources/main.css" rel="stylesheet" />
	
	<script src="resources/ace/ace.js" type="text/javascript" charset="utf-8"></script>
	<script src="resources/app.js" type="text/javascript" charset="utf-8"></script>
	<script src="resources/viz.js" type="text/javascript" charset="utf-8"></script>
	<script src="resources/jquery.min.js" type="text/javascript" charset="utf-8"></script>
</head>

<body>
	<div id="main">
		<div id="header">
			<div id="logo">
				<div id="logo_text">
					<h1><a href="index.php">CoxIter<span class="logo_colour">Web</span></a></h1>
					<h2>A web client for CoxIter</h2>
				</div>
			</div>
			<div id="menubar">
				<ul id="menu">
					<!-- put class="selected" in the li tag for the selected page - to highlight which page you're on -->
					<li class="selected"><a href="index.html">Client</a></li>
					<li><a href="https://github.com/rgugliel/CoxIterWeb" target="_blank">Source code</a></li>
					<li><a href="https://github.com/rgugliel/CoxIter" target="_blank">CoxIter</a></li>
				</ul>
			</div>
		</div>
		<div id="content_header"></div>
		<div id="site_content">
			<div class="sidebar">
				<h1>Visualization</h1>
				<div id="graphParsingErrors">Parsing errors</div>
				<div id="graphImage"></div>
			</div>
			<div id="content">
				<h1>Coxeter graph and invariants<div id="helpButton" title="Help"><a href="#popupHelp"><img src="resources/help.png" /></a></div></h1>
			
				<div id="graphRaw">dimension 4
1 2 3
2 s0 3
s0 s1 3
s0 s2 4</div>
			<div class="actions">
				<div id="downloadGraph" onclick="download('coxiter');">Download graph</div>
				<div id="doComputations" onclick="computeInvariants();">Compute the invariants</div>
			</div><br />
			
			<div id="invariants">
				<h2>Invariants</h2>
				<ul id="invariantsList">
				</ul>
				
				<div class="actions" id="downloads">
					<div id="downloadGraph" onclick="download('coxiter');">Download graph</div>
					<div id="downloadInvariants" onclick="download('invariants');">Download invariants</div>
				</div>
			</div>
			</div>
		</div>
		<div id="content_footer"></div>
		<div id="footer">
			<p>Design: <a href="http://www.html5webtemplates.co.uk">shadowplay_2</a> | CoxIter: Rafael Guglielmetti | <a href="#popupLicense">License</a></p>
		</div>
	</div>
	
	<div id="popupHelp" class="overlay">
		<div class="popup">
			<h2>Describing the graph</h2>
			<a class="close" href="#">&times;</a>
			<div class="content">
				Use the editor to describe the Coxeter group of your hyperbolic Coxeter group/polyhedron.<br /><br />
				
				<ul type="disc">
					<li>The graph can contain up to <?php echo $config['maximalNumberVertices']; ?> vertices</li>
					<li>Use a weight 0 to specify a bold edge</li>
					<li>Use a weight 1 to specify a dotted edge</li>
				</ul>
				
				Once you are done, click on the "Compute the invariants" button!
			</div>
		</div>
	</div>
	
	<div id="popupLicense" class="overlay">
		<div class="popup">
			<h2>License and collected data</h2>
			<a class="close" href="#">&times;</a>
			<div class="content">
				<h3>License</h3>
				This file is part of <a href="https://github.com/rgugliel/CoxIterWeb">CoxIterWeb</a>, which is based on <a href="https://github.com/rgugliel/CoxIter">CoxIter</a>.<br /><br />

				CoxIter is free software: you can redistribute it and/or modify<br />
				it under the terms of the GNU General Public License as<br />
				published by the Free Software Foundation, either version 3 of the<br />
				License, or (at your option) any later version.<br /><br />

				CoxIter is distributed in the hope that it will be useful,<br />
				but WITHOUT ANY WARRANTY; without even the implied warranty of<br />
				MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the<br />
				GNU General Public License for more details.<br />

				You should have received a copy of the GNU General Public License<br />
				along with CoxIter. If not, see <a hreh="http://www.gnu.org/licenses/">http://www.gnu.org/licenses/</a><br /><br />
				
				<h3>Collected data</h3>
				When computation are done, we save the following information:
				<ul>
					<li>date and time</li>
					<li>number of vertices of the graph</li>
					<li>duration of the computations</li>
				</ul>
				We do not save the graph you enter.
			</div>
		</div>
	</div>
</body>

<script type="text/javascript">
var maximalNumberVertices = <?php echo $config['maximalNumberVertices']; ?>;
var maximalWeight = <?php echo $config['maximalWeight']; ?>;
var updateGraphTimeout = 1000;

var editor = ace.edit("graphRaw");
var timerEdit;
editor.on("change", function() {
	editorContentChanged = true;
	clearTimeout( timerEdit );
	timerEdit = setTimeout(updateGraph, updateGraphTimeout);
	
	removeChildren($("#invariantsList"));
	$("#downloads").hide(400);
});

updateGraph();
</script>

</html>
