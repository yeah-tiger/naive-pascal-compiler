var svg = d3.select("svg"),
    width = +svg.attr("width"),
    height = +svg.attr("height"),
    g = svg.append("g").attr("transform", "translate(40,0)");

var tree = d3.tree()
    .size([height, width - 160]);

// var stratify = d3.stratify()
//     .parentId(function(d) { return d.id.substring(0, d.id.lastIndexOf(".")); });
// var stratify = d3.stratify()
//     .parentId(function(d) { return d.substring(0, d.lastIndexOf(".")); });


let prefix = ""

const unwrap = (prefix, data) => {
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
            else
                buffer = buffer.concat(process(data[key], depth+1));
        }
    }
    let w1 = unwrap(data["type"], buffer);
    if (data.hasOwnProperty("children")) {
        data["children"].forEach((u, i) => {
            w1 = w1.concat(unwrap(data["type"] + "__" + data["type"] + "_" + i, u));
        });
    }
    return w1;
}

const stratify = d3.stratify()
.id(function(d) { return d.name; })
.parentId(function(d) { return d.parent; });

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

  console.log(data.filter(e => e.parent === ""))

  var root = stratify(data)
    //   .sort(function(a, b) { return (a.height - b.height) || a.id.localeCompare(b.id); });

  var link = g.selectAll(".link")
    .data(tree(root).links())
    .enter().append("path")
      .attr("class", "link")
      .attr("d", d3.linkHorizontal()
          .x(function(d) { return d.y; })
          .y(function(d) { return d.x; }));

  var node = g.selectAll(".node")
    .data(root.descendants())
    .enter().append("g")
      .attr("class", function(d) { return "node" + (d.children ? " node--internal" : " node--leaf"); })
      .attr("transform", function(d) { return "translate(" + d.y + "," + d.x + ")"; })

  node.append("circle")
      .attr("r", 2.5);

  node.append("text")
      .attr("dy", 3)
      .attr("x", function(d) { return d.children ? -8 : 8; })
      .style("text-anchor", function(d) { return d.children ? "end" : "start"; })
      .text(function(d) { return d.id.substring(d.id.lastIndexOf("__") + 2); });
});