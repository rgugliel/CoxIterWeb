var coxeterMatrix = [];
var coxeterMatrixInitialized = [];
var dimension = 0;

var result_CoxIter = "";
var result_invariants = "";

var runningComputation = false; // True if we are waiting for results to come back
var editorContentChanged = true; // Changed since the last computations

function updateGraph()
{
	removeChildren($("#invariantsList"));
	var content = editor.getValue();
	var rows = editor.getValue().split("\n");
	var rowsCount = rows.length
	
	// -------------------------------------
	// Initializations
	var dimensionSet = false;
	var labelsToIndices = new Object();
	var indicesToLabels = [];
	var verticesCount = 0;
	coxeterMatrix = [];
	coxeterMatrixInitialized = [];
	dimension = 0;
	
	// -------------------------------------
	// Parsing
	var regexDimension = /^dimension[ \t]([0-9]+)$/
	var regexWeightedEdge = /^([a-z0-9_\-]+)[ \t]([a-z0-9_\-]+)[ \t]([0-9]+)$/
	var debugContent = "";
	
	var vertex1, vertex2, weight, str;
	
	for (var i = 0; i < rowsCount; i++ )
	{
		rows[i] = rows[i].replace(/^\s+/g,'').replace(/\s+$/g,'');
		if (rows[i] == "" )
			continue;
			
		if (regexDimension.test(rows[i])) // the dimension
		{
			dim = parseInt(RegExp.$1);
			if (dim < 2)
				return parsingError(i, "dimension invalid");
			else if (dimensionSet && dimension != dim)
				return parsingError(i, "dimension multiple");
				
			dimension = dim;
			dimensionSet = true;
			debugContent += (i + 1) + ": dimension: " + dimension + "<br />";
		}
		else if(regexWeightedEdge.test(rows[i])) // one edge
		{
			if (RegExp.$1 == RegExp.$2)
				return parsingError(i, "vertices same");
				
			if (!(RegExp.$1 in labelsToIndices))
			{
				labelsToIndices[RegExp.$1] = verticesCount++;
				indicesToLabels.push(RegExp.$1);
			}
			if (!(RegExp.$2 in labelsToIndices))
			{
				labelsToIndices[RegExp.$2] = verticesCount++;
				indicesToLabels.push(RegExp.$2);
			}	
			
			if (verticesCount > maximalNumberVertices)
				return parsingError(i, "vertices maximal number");
				
			vertex1 = labelsToIndices[RegExp.$1];
			vertex2 = labelsToIndices[RegExp.$2];
			weight = parseInt(RegExp.$3);
			
			if (weight < 0)
				return parsingError(i, "weight negative");
			else if (weight > maximalWeight)
				return parsingError(i, "weight maximal");
				
			str = addEdge(vertex1, vertex2, weight);
			if (str != "")
				return parsingError(i, str);
				
			debugContent += (i + 1) + ": edge:" + RegExp.$1 + ";" + RegExp.$2 + ";" + RegExp.$3 + "<br />";
		}
		else
			return parsingError(i, "parse");
	}
	
	$("#graphParsingErrors").hide(400);
	
	if (verticesCount > 0)
		drawGraph(verticesCount, indicesToLabels)
		
	// -------------------------------------
	// Updating CoxIter file
	result_CoxIter = verticesCount + " " + (dimension > 0 ? dimension : "") + "\n";
	result_CoxIter += "vertices labels:";
	for (var i = 0; i < verticesCount; i++ )
		result_CoxIter += " " + indicesToLabels[i];
	result_CoxIter += "\n";	
	for (var r = 0; r < verticesCount; r++)
	{
		for (var c = r + 1; c < verticesCount; c++)
		{
			if (coxeterMatrix[r][c] != 2)
				result_CoxIter += indicesToLabels[r] + " " + indicesToLabels[c] + " " + coxeterMatrix[r][c] + "\n";
		}
	}
}

function drawGraph(verticesCount, indicesToLabels)
{
	var strGraph = "strict graph {";
	var strSupp;
	
	for (var r = 0; r < verticesCount; r++)
	{
		for (var c = r + 1; c < verticesCount; c++)
		{
			if (coxeterMatrix[r][c] != 2)
			{
				strSupp = "";
				if (coxeterMatrix[r][c] > 3)
					strSupp = "[label=\"" + coxeterMatrix[r][c] + "\"]";
				else if (coxeterMatrix[r][c] == 1)
					strSupp = "[style=dotted]";
				else if (coxeterMatrix[r][c] == 0)
					strSupp = "[label=\"inf\"]";
					
				strGraph += "\"" + indicesToLabels[r] + "\" -- \"" + indicesToLabels[c] + "\" " + strSupp + ";";
			}
		}
	}
	
	strGraph += "}";
	
	// -----------------------------------
	var graph = document.querySelector("#graphImage");
	var svg = graph.querySelector("svg");
	if (svg)
		graph.removeChild(svg);
		
	var result = Viz(strGraph);
	document.getElementById("graphImage").innerHTML = result;
}

function removeChildren($el) 
{
	if ($el.children().length) 
	{
		$el.children().each(function(i, val) {
			removeChildren($(val));
			$(val).remove();
		});
	}
}

function computeInvariants()
{
	if (runningComputation)
	{
		alert("Please wait until the computations are finished");
		return;
	}
	
	if (!editorContentChanged) // No need to compute
		return;
	
	// ------------------------------------------
	// Graph
	var strGraph = "";
	var verticesCount = coxeterMatrix.length;
	var strNeighbours;
	
	for (var c = 0; c < verticesCount; c++)
	{
		strNeighbours = "";
		for (var r = c + 1; r < verticesCount; r++ )
		{
			if (coxeterMatrix[r][c] != 2)
				strNeighbours += (strNeighbours != "" ? "," : "") + "[" + r + "," + coxeterMatrix[r][c] + "]";
		}
		
		if (strNeighbours != "")
			strGraph += (strGraph != "" ? ";" : "") + "[" + c + "," + strNeighbours + "]";
	}
	
	if (strGraph == "")
		return;
	
	removeChildren($("#invariantsList"));
	result_invariants = "";
	
	// ------------------------------------------
	// Let's go
	runningComputation = true;
	$.ajax({
		url: "resources/computeInvariants.php",
		type: "POST",
		data: "graph=" + strGraph + "&dimension=" + dimension + "&verticesCount=" + verticesCount,
		dataType: "xml",
		success: function(result) {
			var temp;
			
			runningComputation = false;
			
			if ($(result).find('error').length)
				return parsingError(-1, $(result).find('error').text());
				
			if ($(result).find('dimensionGuessed').length)
			{
				$("#invariantsList").append('<li>Guessed dimension: ' + $(result).find('dimensionGuessed').text() + '</li>');
				result_invariants += 'Guessed dimension: ' + $(result).find('dimensionGuessed').text() + "\n";
			}
			
			if ($(result).find('cocompact').length)
			{
				temp = $(result).find('cocompact').text();
				$("#invariantsList").append('<li>Cocompact: ' + (temp == 1 ? "yes" : (temp == 0 ? "no" : "?")) + '</li>');
				result_invariants += 'Cocompact: ' + (temp == 1 ? "yes" : (temp == 0 ? "no" : "?")) + "\n";
			}
			
			if ($(result).find('cofinite').length)
			{
				temp = $(result).find('cofinite').text();
				$("#invariantsList").append('<li>Cofinite: ' + (temp == 1 ? "yes" : (temp == 0 ? "no" : "?")) + '</li>');
				result_invariants += 'Cofinite: ' + (temp == 1 ? "yes" : (temp == 0 ? "no" : "?")) + "\n";
			}
			
			if ($(result).find('fvector').length)
			{
				$("#invariantsList").append('<li>f-vector: ' + $(result).find('fvector').text() + '</li>');
				result_invariants += 'f-vector: ' + $(result).find('fvector').text() + "\n";
			}
				
			if ($(result).find('vatinfinity').length)
			{
				$("#invariantsList").append('<li>Number of vertices at infinity: ' + $(result).find('vatinfinity').text() + '</li>');
				result_invariants += 'Number of vertices at infinity: ' + $(result).find('vatinfinity').text() + "\n";
			}
				
			if ($(result).find('euler').length)
			{
				$("#invariantsList").append('<li>Euler characteristic: ' + $(result).find('euler').text() + '</li>');
				result_invariants += 'Euler characteristic: ' + $(result).find('euler').text() + "\n";
			}
			
			if ($(result).find('covolume').length)
			{
				$("#invariantsList").append('<li>Covolume: ' + $(result).find('covolume').text() + '</li>');
				result_invariants += 'Covolume: ' + $(result).find('covolume').text() + "\n";
			}
			
			if ($(result).find('growthRate').length)
			{
				var gr = $(result).find('growthRate');
				var lis = "";
				
				result_invariants += "Growth rate:\n";
				
				if ($(gr).find('value').length)
				{
					lis += "<li>Value: " + $(gr).find('value').text() + "</li>";
					result_invariants += "\tValue: " + $(gr).find('value').text() + "\n";
				}
				
				if ($(gr).find('perron').length)
				{
					lis += '<li>Perron number: ' + $(gr).find('perron').text() + '</li>';
					result_invariants += "\tPerron number: " + $(gr).find('perron').text() + "\n";
				}
					
				if ($(gr).find('pisot').length)
				{
					lis += '<li>Pisot number: ' + $(gr).find('pisot').text() + '</li>';
					result_invariants += "\tPisot number: " + $(gr).find('pisot').text() + "\n";
				}
					
				if ($(gr).find('salem').length)
				{
					lis += '<li>Salem number: ' + $(gr).find('salem').text() + '</li>';
					result_invariants += "\tSalem number: " + $(gr).find('salem').text() + "\n";
				}
				
				$("#invariantsList").append("<li>Growth rate: <ul>" + lis + "</ul></li>");
			}
			
			if ($(result).find('numerator').length && $(result).find('denominator').length)
				result_invariants += "\nf(x)=C(" + $(result).find('numerator').text() + ")/(" +  $(result).find('denominator').text() + ")";
				
			$("#downloads").show(400);
			editorContentChanged = false;
		},
		error: function(errorData) { 
			parsingError(-1, 'ajax');
		}
	});
}

function download(source)
{
	if (source != "coxiter" && source != "invariants")
		return;
		
	var element = document.createElement('a');
	element.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(source == "coxiter" ? result_CoxIter : result_invariants));
	element.setAttribute('download', source == "coxiter" ? 'graph.coxiter' : 'invariants.txt');

	element.style.display = 'none';
	document.body.appendChild(element);

	element.click();

	document.body.removeChild(element);result_
}

/*! \fn addEdge
 * \brief Add an edge to the Coxeter matrix
 * 
 * \param vertex1(int >=0) Index of the first vertex
 * \param vertex2(int >=0) Index of the second vertex
 * \param weight(int >=0) Weight
 * 
 * \return Empty string if OK, error code otherwise
 */
function addEdge(vertex1, vertex2, weight)
{
	var vmax = Math.max(vertex1, vertex2);
	
	// Do we have to add rows and columns?
	if (vmax >= coxeterMatrix.length)
	{
		// Completing the rows
		for (var r = 0; r < coxeterMatrix.length; r++)
		{
			for (var c = coxeterMatrix.length; c <= vmax; c++)
			{
				coxeterMatrix[r].push(2);
				coxeterMatrixInitialized[r].push(0);
			}
		}
		
		// Adding some row
		for (var r = coxeterMatrix.length; r <= vmax; r++)
		{
			coxeterMatrix.push([]);
			coxeterMatrixInitialized.push([]);
			for (var c = 0; c <= vmax; c++)
			{
				coxeterMatrix[r].push(2);
				coxeterMatrixInitialized[r].push(0);
			}	
		}
	}
	
	if (coxeterMatrixInitialized[vertex1][vertex2] && coxeterMatrix[vertex1][vertex2] != weight)
		return "edge weight multiple";

	coxeterMatrix[vertex1][vertex2] = coxeterMatrix[vertex2][vertex1] = weight;
	coxeterMatrixInitialized[vertex1][vertex2] = coxeterMatrixInitialized[vertex2][vertex1] = 1;
	
	return "";
}

function parsingError(rowIndex, errorCode)
{
	var strError = "";
	
	if(rowIndex >= 0)
		strError += "Error in line " + (rowIndex + 1) + "<br />";
		
	if (errorCode ==  "ajax")
		strError += "One error occurred during the computation of the invariants";
	else if (errorCode ==  "dimension invalid")
		strError += "The dimension should be greater than one";
	else if (errorCode ==  "dimension multiple")
		strError += "The dimension was already defined";
	else if (errorCode ==  "edge weight multiple")
		strError += "This edge was already defined";
	else if (errorCode ==  "graph empty")
		strError += "Please give a graph";
	else if (errorCode ==  "graph encoding")
		strError += "Check the encoding of your graph; maybe the group is not of finite covolume";
	else if (errorCode ==  "parse")
		strError += "Unable to parse line";
	else if (errorCode == "vertices maximal number")
		strError += "The number of vertices is limited to " + maximalNumberVertices;
	else if (errorCode ==  "vertices same")
		strError += "The specified vertices are the same";
	else if (errorCode ==  "weight maximal")
		strError += "The weight should be less than " + (maximalWeight + 1);
	else if (errorCode ==  "weight negative")
		strError += "The weight should be non-negative";
	else
		strError += "Unknown error";
		
	document.getElementById("graphParsingErrors").innerHTML = strError;
	$("#graphParsingErrors").show(400);

}
