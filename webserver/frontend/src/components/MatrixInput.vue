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
            <a-row :gutter="24">
                <a-col :span="4">
                    <a-button v-on:click="negative" style="width: 100%">negative</a-button>
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
                ]
            }
        },
        methods: {
            negative: unaryOperate("/api/negative"),
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