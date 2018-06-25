const unwrap = (prefix, data) => {
    if (prefix === null) return data
    return [prefix].concat(data.map(u => {
        return prefix + "__" + u
    }))
}

const flatten = data => {
    if (isString(data)) return data;
    let buffer = [];
    for (var key in data) {
        if (data.hasOwnProperty(key)) {
            buffer.push(flatten(data[key]));
        }
    }
    return buffer.join(', ');
}
const process = (data, depth) => {
    let buffer = new Array();
    for (let key in data) {
        if (key === "type") continue;
        if (key === "children") {
            data[key] = data[key].map(u => {
                if (isString(u)) return u;
                return process(u, depth+1);
            });
            continue;
        }
        if (data.hasOwnProperty(key)) {
            if (isString(data[key]))
                buffer = buffer.concat(key + " " + data[key]);
            else if (key in ["Identifier"]) {
                buffer = buffer.concat(key + " " + data[key]);
            }
            else
                buffer = buffer.concat(process(data[key], depth+1));
        }
    }
    let w1 = unwrap(data["type"], buffer);
    if (data["type"] == "VarDecl") w1 = unwrap(null, buffer);
    if (data.hasOwnProperty("children") && isArray(data["children"]) && data["children"].length > 0) {
        let type;
        if (data["type"] === "Program") type = data["type"] + "__Routinebody";
        else type = data["type"];
        w1 = w1.concat(type)
        data["children"].forEach((u, i) => {
            w1 = w1.concat(unwrap(type + "__" + data["type"] + "_" + i, u));
        });
    }
    return w1;
}

const stratify = d3.stratify()
.id(function(d) { return d.name; })
.parentId(function(d) { return d.parent; });





  // Set the dimensions and margins of the diagram
  var margin = {top: 20, right: 90, bottom: 30, left: 90},
  width = 2800 - margin.left - margin.right,
  height = 950 - margin.top - margin.bottom;

  // append the svg object to the body of the page
  // appends a 'group' element to 'svg'
  // moves the 'group' element to the top left margin
  var svg = d3.select("body").append("svg")
  .attr("width", width + margin.right + margin.left)
  .attr("height", height + margin.top + margin.bottom)
  .append("g")
  .attr("transform", "translate("
        + margin.left + "," + margin.top + ")");

  var i = 0,
  duration = 750;

  // declares a tree layout and assigns the size
  var treemap = d3.tree().size([height, width]);




var root;


// d3.csv("flare.csv", function(error, data) {
d3.json("out.json", function(error, data) {
  if (error) throw error;

  data = process(data, 0);

  data = data.filter( (value, index, self) => self.indexOf(value) === index );
  data = data.map(d => {
      return {
          'name': d,
          'parent': d.substring(0, d.lastIndexOf("__"))
      }
  });

  root = stratify(data);

  ///////////////////////////////////////////////////////

// Assigns parent, children, height, depth
// root = d3.hierarchy(treeData, function(d) { return d.children; });
root.x0 = height / 2;
root.y0 = 0;

// Collapse after the second level
root.children.forEach(collapse);
root.children.forEach(d => {
    if (d._children === undefined) return;
    d.children = d._children;
    d._children = null;
})

update(root);
})
// Collapse the node and all it's children
function collapse(d) {
    if(d.children) {
      d._children = d.children
      d._children.forEach(collapse)
      d.children = null
    }
  }

function update(source) {

    // Assigns the x and y position for the nodes
    var treeData = treemap(root);

    // Compute the new tree layout.
    var nodes = treeData.descendants(),
        links = treeData.descendants().slice(1);

    // Normalize for fixed-depth.
    nodes.forEach(function(d){ d.y = d.depth * 180});

    // ****************** Nodes section ***************************

    // Update the nodes...
    var node = svg.selectAll('g.node')
        .data(nodes, function(d) {return d.id || (d.id = ++i); });

    // Enter any new modes at the parent's previous position.
    var nodeEnter = node.enter().append('g')
        .attr('class', 'node')
        .attr("transform", function(d) {
          return "translate(" + source.y0 + "," + source.x0 + ")";
      })
      .on('click', click);

    // Add Circle for the nodes
    nodeEnter.append('circle')
        .attr('class', 'node')
        .attr('r', 1e-6)
        .style("fill", function(d) {
            return d._children ? "lightsteelblue" : "#fff";
        });

    // Add labels for the nodes
    nodeEnter.append('text')
        .attr("dy", ".35em")
        .attr("x", function(d) {
            return d.children || d._children ? -13 : 13;
        })
        .attr("text-anchor", function(d) {
            return d.children || d._children ? "end" : "start";
        })
        // .text(function(d) { return d.data.name; });
        .text(function(d) {
            if (d.id.lastIndexOf("__") === -1) return d.id.substring(0);
            return d.id.substring(d.id.lastIndexOf("__") + 2);
        });

    // UPDATE
    var nodeUpdate = nodeEnter.merge(node);

    // Transition to the proper position for the node
    nodeUpdate.transition()
      .duration(duration)
      .attr("transform", function(d) {
          return "translate(" + d.y + "," + d.x + ")";
       });

    // Update the node attributes and style
    nodeUpdate.select('circle.node')
      .attr('r', 10)
      .style("fill", function(d) {
          return d._children ? "lightsteelblue" : "#fff";
      })
      .attr('cursor', 'pointer');


    // Remove any exiting nodes
    var nodeExit = node.exit().transition()
        .duration(duration)
        .attr("transform", function(d) {
            return "translate(" + source.y + "," + source.x + ")";
        })
        .remove();

    // On exit reduce the node circles size to 0
    nodeExit.select('circle')
      .attr('r', 1e-6);

    // On exit reduce the opacity of text labels
    nodeExit.select('text')
      .style('fill-opacity', 1e-6);

    // ****************** links section ***************************

    // Update the links...
    var link = svg.selectAll('path.link')
        .data(links, function(d) { return d.id; });

    // Enter any new links at the parent's previous position.
    var linkEnter = link.enter().insert('path', "g")
        .attr("class", "link")
        .attr('d', function(d){
          var o = {x: source.x0, y: source.y0}
          return diagonal(o, o)
        });

    // UPDATE
    var linkUpdate = linkEnter.merge(link);

    // Transition back to the parent element position
    linkUpdate.transition()
        .duration(duration)
        .attr('d', function(d){ return diagonal(d, d.parent) });

    // Remove any exiting links
    var linkExit = link.exit().transition()
        .duration(duration)
        .attr('d', function(d) {
          var o = {x: source.x, y: source.y}
          return diagonal(o, o)
        })
        .remove();

    // Store the old positions for transition.
    nodes.forEach(function(d){
      d.x0 = d.x;
      d.y0 = d.y;
    });

    // Creates a curved (diagonal) path from parent to the child nodes
    function diagonal(s, d) {

      path = `M ${s.y} ${s.x}
              C ${(s.y + d.y) / 2} ${s.x},
                ${(s.y + d.y) / 2} ${d.x},
                ${d.y} ${d.x}`

      return path
    }

    function show(d) {
        if (!d.children) {
            d.children = d._children;
            d._children = null;
            if (d.children) d.children.forEach(show)
        } else {
            d._children = d.children;
            d.children = null;
            if (d._children) d._children.forEach(show)
        }
    }

    // Toggle children on click.
    function click(d) {
        if (d.depth >= 2) {
            show(d);
        }
        else if (d.children) {
            d._children = d.children;
            d.children = null;
        } else {
            d.children = d._children;
            d._children = null;
        }
        update(d);
    }
}
