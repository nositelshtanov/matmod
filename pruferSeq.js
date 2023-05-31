const process = require("process");
const fs = require("fs/promises");

/*
Input 
[
    [1, 1, 1, 3, 2, 2, 5, 6, 6],
    [4, 3, 2, 10, 5, 6, 7, 8, 9]
]
Outpul
    [1, 5, 2, 6, 6, 2, 1, 3]
*/

program(process.argv[2], process.argv[3], process.argv[4]);

function program(command, fileName, targetFileName) {
    if (!fileName || !targetFileName || !command) return;

    if (command == 'encode') {
        fs.readFile(fileName, "utf8")
        .then((data) => {
            const tree = readTree(data);
            const prypherCode = encodePrypher(tree);

            return fs.writeFile(targetFileName, prypherCode.join(" "));
        })
        .catch(err => console.log(err));

    } else if (command == "decode") {
        fs.readFile(fileName, "utf-8")
        .then((data) => {
            const prypher = data.split(" ").map(el => Number(el));
            const tree = decodePrypher(prypher);
            const strTree = [tree[0].join(" "), tree[1].join(" ")].join("\r\n");

            return fs.writeFile(targetFileName, strTree);
        })
        .catch(err => console.log(err));
    }
}

function readTree(data) {
    const [str1, str2] = data.split("\r\n");
    const tree = [];

    tree.push(str1.split(" "));
    tree.push(str2.split(" "));

    return tree;
}

function decodePrypher(prypher) {
    const prypherLen = prypher.length;
    const treeNodesCount = prypher.length + 2;
    const tree = [[], []];
    const restoreNodes = [];

    for (let i = 0; i < prypherLen; i++) {
        const minNode = getMinNotExistsNode(prypher, restoreNodes, treeNodesCount);
        tree[0].push(prypher.shift());
        tree[1].push(minNode);
        restoreNodes.push(minNode);
    }

    const a = getMinNotExistsNode(prypher, restoreNodes, treeNodesCount);
    restoreNodes.push(a);

    const b = getMinNotExistsNode(prypher, restoreNodes, treeNodesCount);

    tree[1].push(a);
    tree[0].push(b);

    return tree;
}

function getMinNotExistsNode(prypher, restoreNodes, treeNodesCount) {
    const notExistsNodes = [];

    for (let i = 1; i <= treeNodesCount; i++) {
        if (!prypher.includes(i) && !restoreNodes.includes(i)) {
            notExistsNodes.push(i);
        }
    }    

    return Math.min(...notExistsNodes);
}



function printTree(tree) {
    for (let i = 0; i < tree[0].length; i++) {
        process.stdout.write(tree[0][i] + " ");
    }

    console.log();

    for (let i = 0; i < tree[0].length; i++) {
        process.stdout.write(tree[1][i] + " ");
    }
}


function encodePrypher(tree) {
    const prypher = [];
    const prypherLen = tree[0].length - 2;

    for (let i = 0; i <= prypherLen; i++) {
        encodeIteration(tree, prypher);
    }

    return prypher;
}

function encodeIteration(tree, prypher) {
    const minIndex = getMinIndex(tree);

    prypher.push(tree[0][minIndex]);

    tree[0].splice(minIndex, 1);
    tree[1].splice(minIndex, 1);
}

function getMinIndex(tree) {
    let minIndex;

    for (let i = 0; i < tree[0].length; i++) {
        if (minIndex === undefined && isLeaf(tree, tree[1][i])) {
            minIndex = i;
        } else if (tree[1][i] < tree[1][minIndex] && isLeaf(tree, tree[1][i])) {
            minIndex = i;
        }
    }

    return minIndex;
}

function isLeaf(tree, targetNode) {
    return tree[0].filter((node) => node === targetNode).length === 0;
}
