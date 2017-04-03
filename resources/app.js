var coxeterMatrix = [];
var dimension = 0;

function updateGraph()
{
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
			else if (weight > 255)
				return parsingError(i, "weight maximal");
				
			str = addEdge(vertex1, vertex2, weight);
			if (str != "")
				return parsingError(i, str);
				
			debugContent += (i + 1) + ": edge:" + RegExp.$1 + ";" + RegExp.$2 + ";" + RegExp.$3 + "<br />";
		}
		else
			return parsingError(i, "parse");
	}
	
	document.getElementById("graphParsingErrors").style.display = "none";
	
	if (verticesCount > 0)
		drawGraph(verticesCount, indicesToLabels)
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

function computeInvariants()
{
	$.ajax({
		url: "resources/computeInvariants.php",
		type: "GET",
		data: "graph=" + strGraph + "&dimension=" + dimension + "&verticesCount=" + verticesCount,
		success: function(result) {
			// TODO;
		},
		error: function(errorData) { 
		// TODO
		}
	});
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
			for (var c = coxeterMatrix.length; c <= vmax; c++ )
				coxeterMatrix[r].push(2);
		}
		
		// Adding some row
		for (var r = coxeterMatrix.length; r <= vmax; r++)
		{
			coxeterMatrix.push([]);
			for (var c = 0; c <= vmax; c++)
				coxeterMatrix[r].push(2);
		}
	}
	
	if (coxeterMatrix[vertex1][vertex2] != 2 && coxeterMatrix[vertex1][vertex2] != weight)
		return "edge weight multiple";

	coxeterMatrix[vertex1][vertex2] = coxeterMatrix[vertex2][vertex1] = weight;
	
	return "";
}

function parsingError(rowIndex, errorCode)
{
	var strError = "Error in line " + (rowIndex + 1) + "<br />";
	
	if (errorCode ==  "dimension invalid")
		strError += "The dimension should be greater than one";
	else if (errorCode ==  "dimension multiple")
		strError += "The dimension was already defined";
	else if (errorCode ==  "edge weight multiple")
		strError += "This edge was already defined";
	else if (errorCode ==  "parse")
		strError += "Unable to parse line";
	else if (errorCode == "vertices maximal number")
		strError += "The number of vertices is limited to " + maximalNumberVertices;
	else if (errorCode ==  "vertices same")
		strError += "The specified vertices are the same";
	else if (errorCode ==  "weight maximal")
		strError += "The weight should less than 256";
	else if (errorCode ==  "weight negative")
		strError += "The weight should be non-negative";
	else
		strError += "Unknown error";
		
	document.getElementById("graphParsingErrors").innerHTML = strError;
	document.getElementById("graphParsingErrors").style.display = "block";
}
