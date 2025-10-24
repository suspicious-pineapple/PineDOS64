
function findPatterns(grid){
    let cgrid = JSON.parse(JSON.stringify(grid));
    let drawInstructions = [];

    //sort patterns by area, largest first, but with a small random factor
    
    patterns = patterns.sort((a,b) => {
        let areaA = a.getArea();
        let areaB = b.getArea();
        let rndA = Math.random()*1.5;
        let rndB = Math.random()*1.5;
        return areaB + rndB - areaA - rndA;
    });
    

    //loop through all patterns
    for(let a = 0; a < patterns.length; a++){
        //correlate the pattern to the grid
        let cpattern = patterns[a].pattern;
        let patternWidth = cpattern[0].length;
        let patternHeight = cpattern.length;
        let gridWidth = cgrid[0].length;
        let gridHeight = cgrid.length;

        //loop through the grid
        for(let y = 0; y < gridHeight-patternHeight+1; y++){
            for(let x = 0; x < gridWidth-patternWidth+1; x++){
                let match = true;
                for(let px=0; px<patternWidth; px++){
                    for(let py=0; py<patternHeight; py++){
                        if(cpattern[py][px] != cgrid[y+py][x+px]){
                            match = false;
                        }
                    }
                }
                if(match){
                    //set the cells to 0
                    for(let px=0; px<patternWidth; px++){
                        for(let py=0; py<patternHeight; py++){
                            if(Math.random() < 0.999){
                                cgrid[y+py][x+px] = 0;
                            }
                     }
                    }
                    //add the draw instruction
                    drawInstructions.push({pattern:patterns[a],x:x, y:y, code: patterns[a].drawCode(x,y)});
                }
            }
        }    
    }


    //remove overlapping draw instructions
    let newDrawInstructions = [];
    for(let i = 0; i < drawInstructions.length; i++){
        let overlap = false;
        for(let j = 0; j < drawInstructions.length; j++){
            if(i != j){
                if(drawInstructions[i].x >= drawInstructions[j].x && drawInstructions[i].x < drawInstructions[j].x + drawInstructions[j].pattern.pattern[0].length && drawInstructions[i].y >= drawInstructions[j].y && drawInstructions[i].y < drawInstructions[j].y + drawInstructions[j].pattern.pattern.length){
                    overlap = true;
                }
            }
        }
        if(!overlap){
            newDrawInstructions.push(drawInstructions[i]);
        }
    }











    return drawInstructions;

}

//random boolean generator with seed
function* seedRandom(seed) {
    while (true) {
        seed = (seed * 9301 + 49297) % 233280;
        yield seed / 233280;
    }
}



class pattern {
    constructor(pattern, drawCode){
        this.pattern = pattern;
        this.drawCode = drawCode;
    }
    getArea(){
        //sum all the numbers in the pattern
        let area = 0;
        for(let i = 0; i < this.pattern.length; i++){
            for(let j = 0; j < this.pattern[i].length; j++){
                area += this.pattern[i][j];
            }
        }
        return area;
    }
    drawPreview(canvas,x,y){
        //random color
        let color = "rgb("+Math.floor(Math.random()*255)+","+Math.floor(Math.random()*255)+","+Math.floor(Math.random()*255)+")";


        let ctx = canvas.getContext("2d");
        ctx.fillStyle = color;

        //draw the pattern
        for(let i = 0; i < this.pattern.length; i++){
            for(let j = 0; j < this.pattern[i].length; j++){
                if(this.pattern[i][j] == 1){
                    ctx.fillRect(x+j,y+i,1,1);
                }
            }
        }

    }

}


let square1 = new pattern([
    [1,1],
    [1,1]
], function(x,y){
    return `
    draw rect gridX${x} gridY${y} 2 2
    `;
})

let circle1 = new pattern([
    [0,1,1,0],
    [1,0,0,1],
    [1,0,0,1],
    [0,1,1,0],
], function(x,y){
    return `
    draw linePoly gridX${x} gridY${y} 4 2 0 0
`;})

let circle2 = new pattern([
    [1,1,1,1],
    [1,0,0,1],
    [1,0,0,1],
    [1,1,1,1],
], function(x,y){
    return `
    draw lineRect gridX${x} gridY${y} 4 4
`;})

let circle3 = new pattern([
    [1,1,1],
    [1,0,1],
    [1,1,1],
], function(x,y){
    return `
    draw lineRect gridX${x} gridY${y} 3 3
`;})
let circle4 = new pattern([
    [1,1,1,1],
    [1,0,0,1],
    [1,0,0,1],
    [1,0,0,1],
    [1,1,1,1],
], function(x,y){
    return `
    draw lineRect gridX${x} gridY${y} 4 5
`;})

let circle5 = new pattern([
    [1,1,1],
    [1,0,1],
    [1,0,1],
    [1,1,1],
], function(x,y){
    return `
    draw lineRect gridX${x} gridY${y} 3 4
`;})



let rect2 = new pattern([
    [1],
    [1],
    [1],
    [1],
]
, function(x,y){
    return `
    draw rect gridX${x} gridY${y} 1 4
`;})
let rect3 = new pattern([
    [1,1,1]
]
, function(x,y){
    return `
    draw rect gridX${x} gridY${y} 3 1
`;})
let rect4 = new pattern([
    [1],
    [1],
    [1],
]
, function(x,y){
    return `
    draw rect gridX${x} gridY${y} 1 3
`;})

let rect5 = new pattern([
    [1,1]
]
, function(x,y){
    return `
    draw rect gridX${x} gridY${y} 2 1
`;})
let rect6 = new pattern([
    [1],
    [1],
]
, function(x,y){
    return `
    draw rect gridX${x} gridY${y} 1 2
`;})

let diag1 = new pattern([
    [1,0,],
    [0,1,],
]
, function(x,y){
    return `
    draw line gridX${x} gridY${y} gridX${x+2} gridY${y+2}
`;})
let diag2 = new pattern([
    [0,1,],
    [1,0,],
]
, function(x,y){
    return `
    draw line gridX${x} gridY${y+2} gridX${x+2} gridY${y}
`;})

let diag3 = new pattern([
    [1,0,0],
    [0,1,0],
    [0,0,1],
]
, function(x,y){
    return `
    draw line gridX${x} gridY${y} gridX${x+3} gridY${y+3}
`;}
)
let diag4 = new pattern([
    [0,0,1],
    [0,1,0],
    [1,0,0],
]
, function(x,y){
    return `
    draw line gridX${x} gridY${y+3} gridX${x+3} gridY${y}
`;}
)


let diag5 = new pattern([
    [1,0,0,0],
    [0,1,0,0],
    [0,0,1,0],
    [0,0,0,1],
]
, function(x,y){
    return `
    draw line gridX${x} gridY${y} gridX${x+4} gridY${y+4}
`;}
)
let diag6 = new pattern([
    [0,0,0,1],
    [0,0,1,0],
    [0,1,0,0],
    [1,0,0,0],
]
, function(x,y){
    return `
    draw line gridX${x} gridY${y+4} gridX${x+4} gridY${y}
`;}
)
let diag7 = new pattern([
    [1,0,0,0,0],
    [0,1,0,0,0],
    [0,0,1,0,0],
    [0,0,0,1,0],
    [0,0,0,0,1],
]
, function(x,y){
    return `
    draw line gridX${x} gridY${y} gridX${x+5} gridY${y+5}
`;}
)
let diag8 = new pattern([
    [0,0,0,0,1],
    [0,0,0,1,0],
    [0,0,1,0,0],
    [0,1,0,0,0],
    [1,0,0,0,0],
]
, function(x,y){
    return `
    draw line gridX${x} gridY${y+5} gridX${x+5} gridY${y}
`;}
)



let rect7 = new pattern([
    [1,1,1,1,1],
]
, function(x,y){
    return `
    draw rect gridX${x} gridY${y} 5 1
`;}
)
let rect8 = new pattern([
    [1],
    [1],
    [1],
    [1],
    [1],
]
, function(x,y){
    return `
    draw rect gridX${x} gridY${y} 1 5
`;}
)
let rect9 = new pattern([
    [1],
    [1],
    [1],
    [1],
    [1],
    [1],
]
, function(x,y){
    return `
    draw rect gridX${x} gridY${y} 1 6
`;})

/*
let dot1 = new pattern([
    [1],
]
, function(x,y){
    return `
    draw rect gridX${x} gridY${y} 1 1
`;})
*/



let shape1 = new pattern([ //"#" shape
    [0,1,0,1,0],
    [1,1,1,1,1],
    [0,1,0,1,0],
    [1,1,1,1,1],
    [0,1,0,1,0],
]
, function(x,y){
    return `
    draw rect gridX${x} gridY${y} 5 2
    draw rect gridX${x} gridY${y+3} 5 2
    draw rect gridX${x} gridY${y} 2 5
    draw rect gridX${x+3} gridY${y} 2 5
`;})

let shape2 = new pattern([ //circle1 with a rectangle on the side
    [1,0,0],
    [1,1,1],
    [1,0,1],
    [1,1,1],
    [1,0,0],
]
, function(x,y){
    return `
    draw rect gridX${x} gridY${y} 1 5
    draw lineRect gridX${x} gridY${y+1} 3 3
`;})
let cross1 = new pattern([
    [1,0,1],
    [0,1,0],
    [1,0,1],
]
, function(x,y){
    return `
    draw line gridX${x} gridY${y} gridX${x+2} gridY${y+2}
    draw line gridX${x} gridY${y+2} gridX${x+2} gridY${y}
`;})
let cross2 = new pattern([
    [1,0,0,0,1],
    [0,1,0,1,0],
    [0,0,1,0,0],
    [0,1,0,1,0],
    [1,0,0,0,1],
]
, function(x,y){
    return `
    draw line gridX${x} gridY${y} gridX${x+4} gridY${y+4}
    draw line gridX${x} gridY${y+4} gridX${x+4} gridY${y}
`;})




let dot1 = new pattern([
    [1],
]
, function(x,y){
    return `
    mov ecx, dword [gridX${x}]
    mov edx, dword [gridY${y}]
    call put_pixel
    ;draw rect gridX${x} gridY${y} 1 1
`;})


let rect1 = new pattern([
    [1,1,1],
]
, function(x,y){
    return `
    mov ebx, 3
    mov cl, byte [scale]
    shl ebx,cl
    mov ecx, dword [gridX${x}]
    mov edx, dword [gridY${y}]
    ; ECX -> X, EDX -> Y, EBX -> Length, EAX -> Color
    call horizontal_line
    ;draw rect gridX${x} gridY${y} 4 1
    
`;})

rect2 = new pattern([
    [1,1],
]
, function(x,y){
    return `
    mov ebx, 2
    mov cl, byte [scale]
    shl ebx,cl
    mov ecx, dword [gridX${x}]
    mov edx, dword [gridY${y}]
    ; ECX -> X, EDX -> Y, EBX -> Length, EAX -> Color
    call horizontal_line
    ;draw rect gridX${x} gridY${y} 4 1
    
`;})

let vert1 = new pattern([
    [1],
    [1]
]
, function(x,y){
    return `
    mov ecx, dword [gridX${x}]
    mov edx, dword [gridY${y}]
    call put_pixel
    inc edx
    inc edx
    call put_pixel
    ;draw rect gridX${x} gridY${y} 1 1
`;})

diag1 = new pattern([
    [0,1],
    [1,0]
]
, function(x,y){
    return `
    mov ecx, dword [gridX${x}]
    mov edx, dword [gridY${y}]
    call put_pixel
    dec edx
    dec edx
    inc ecx
    inc ecx
    call put_pixel
    ;draw rect gridX${x} gridY${y} 1 1
`;})

diag2 = new pattern([
    [1,0],
    [0,1]
]
, function(x,y){
    return `
    mov ecx, dword [gridX${x}]
    mov edx, dword [gridY${y}]
    call put_pixel
    dec ecx
    dec ecx
    inc edx
    inc edx
    call put_pixel
    ;draw rect gridX${x} gridY${y} 1 1
`;})




//sort by area, largest first
//let patterns = [cross1, cross2, shape1, circle5, shape2, square1, circle1, circle2, circle3, circle4, rect1, rect2, rect3, rect4, rect5, rect6, diag1, diag2, diag3, diag4, diag5, diag6,diag7,diag8, rect7, rect8, rect9, dot1].sort((a,b) => b.getArea() - a.getArea());
let patterns = [dot1];


