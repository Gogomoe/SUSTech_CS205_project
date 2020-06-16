<template>
    <div>
        <a-row>
            <a-col :span="3" style="line-height: 32px">
                rows
            </a-col>
            <a-col :span="9">
                <a-input-number v-model="rows" :min="1" :max="10" @change="onMatrixSize"/>
            </a-col>
            <a-col :span="3" style="line-height: 32px">
                columns
            </a-col>
            <a-col :span="9">
                <a-input-number v-model="cols" :min="1" :max="10" @change="onMatrixSize"/>
            </a-col>
        </a-row>
        <div class="matrix-data-input">
            <template v-for="i in Array(rows).keys()">
                <div class="matrix-data-input-line" v-bind:key="`row${i}`">
                    <template v-for="j in Array(cols).keys()">
                        <a-input v-model="matrix[i][j]" style="width: 4em" v-bind:key="`col${j}`"/>
                    </template>
                </div>
            </template>
        </div>
        <div>
            <a-row :gutter="24" style="padding: 0.5em 0">
                <a-col :span="4">
                    <a-button v-on:click="negative" style="width: 100%">negative</a-button>
                </a-col>
                <a-col :span="4">
                    <a-button v-on:click="sum" style="width: 100%">sum</a-button>
                </a-col>
            </a-row>
            <a-row :gutter="24" style="padding: 0.5em 0">
                <a-col :span="2" style="line-height: 32px">
                    <b>slice</b>
                </a-col>
                <a-col :span="2" style="line-height: 32px">
                    row
                </a-col>
                <a-col :span="4" style="line-height: 32px">
                    <a-input v-model="sliceRow" style="width: 100%"/>
                </a-col>
                <a-col :span="2" style="line-height: 32px">
                    col
                </a-col>
                <a-col :span="4" style="line-height: 32px">
                    <a-input v-model="sliceCol" style="width: 100%"/>
                </a-col>
                <a-col :span="4" style="line-height: 32px">
                    <a-button v-on:click="slice" style="width: 100%">slice</a-button>
                </a-col>
            </a-row>
        </div>
    </div>
</template>

<script>

    function unaryOperate(api) {
        return async function () {
            let matrix = this.readMatrix();
            let isNumber = matrix.type === "number";

            if (isNumber) {
                matrix.matrix.data = matrix.matrix.data.map(row => row.map(it => parseFloat(it)));
            }

            let data = {
                type: isNumber ? "number" : "string",
                mat1: matrix.matrix
            };

            let resp = await fetch(api, {
                method: "post",
                body: JSON.stringify(data)
            });
            if (!resp.ok) {
                this.$message.error('Error: operation failed');
                return;
            }
            let json = await resp.json();
            this.$emit('update:result', json.data);
        }
    }

    export default {
        name: "MatrixInput",
        data: function () {
            return {
                rows: 3,
                cols: 3,
                matrix: [
                    ["", "", ""],
                    ["", "", ""],
                    ["", "", ""]
                ],
                sliceRow: "",
                sliceCol: ""
            }
        },
        methods: {
            negative: unaryOperate("/api/negative"),
            sum: unaryOperate("/api/sum"),
            slice: async function () {
                let matrix = this.readMatrix();
                let isNumber = matrix.type === "number";

                if (isNumber) {
                    matrix.matrix.data = matrix.matrix.data.map(row => row.map(it => parseFloat(it)));
                }

                let row, col;
                if (this.sliceRow.trim().length === 0) {
                    row = Array.from(new Array(this.rows).keys());
                } else {
                    row = this.sliceRow.split(/\s+/).map(it => parseInt(it));
                }
                if (this.sliceCol.trim().length === 0) {
                    col = Array.from(new Array(this.cols).keys());
                } else {
                    col = this.sliceCol.split(/\s+/).map(it => parseInt(it));
                }

                let data = {
                    type: isNumber ? "number" : "string",
                    mat1: matrix.matrix,
                    row: row,
                    col: col
                };

                let resp = await fetch("/api/slice", {
                    method: "post",
                    body: JSON.stringify(data)
                });
                if (!resp.ok) {
                    this.$message.error('Error: operation failed');
                    return;
                }
                let json = await resp.json();
                this.$emit('update:result', json.data);
            },
            onMatrixSize() {
                let {rows, cols, matrix} = this;
                while (matrix.length < rows) {
                    matrix.push([])
                }
                for (let row of matrix) {
                    while (row.length < cols) {
                        row.push("")
                    }
                }
            },
            readMatrix: function () {
                let rows = this.rows;
                let cols = this.cols;
                let isNumber = true;

                let result = [];
                for (let i = 0; i < rows; i++) {
                    result.push([]);
                    for (let j = 0; j < cols; j++) {
                        let cell = this.matrix[i][j].trim();
                        if (isNaN(cell)) {
                            isNumber = false;
                        }
                        result[i].push(this.matrix[i][j]);
                    }
                }

                return {
                    type: isNumber ? "number" : "string",
                    matrix: {
                        rows: rows,
                        cols: cols,
                        data: result
                    }
                };
            }
        },
    }
</script>

<style scoped>

    .matrix-data-input {
        width: 100%;
        min-height: 200px;
        display: flex;
        flex-direction: column;
        justify-content: center;
        margin: 1em 0;
        overflow-x: auto;
    }

    .matrix-data-input-line {
        display: flex;
        justify-content: center;
    }

</style>