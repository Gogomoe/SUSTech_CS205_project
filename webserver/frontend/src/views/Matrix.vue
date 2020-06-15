<template>
    <div>
        <h1>Matrix Calculate</h1>
        <a-row>
            <a-col :span="10">
                <h3>Matrix A</h3>
                <MatrixInput ref="matrixA"></MatrixInput>
            </a-col>
            <a-col :span="4">
                <div class="operator-panel">
                    <a-button v-on:click="plus">A + B</a-button>
                    <a-button v-on:click="minus">A - B</a-button>
                    <a-button v-on:click="times">A × B</a-button>
                </div>
            </a-col>
            <a-col :span="10">
                <h3>Matrix B</h3>
                <MatrixInput ref="matrixB"></MatrixInput>
            </a-col>
        </a-row>
        <template v-if="result">
            <h3>Result:</h3>
            <div class="matrix-result">
                <template v-for="i in result.keys()">
                    <div class="matrix-result-line" v-bind:key="`row${i}`">
                        <template v-for="j in result[i].keys()">
                            <span style="display:inline-block; width: 4em" v-bind:key="`col${j}`">
                                {{ result[i][j] }}
                            </span>
                        </template>
                    </div>
                </template>
            </div>
        </template>
    </div>
</template>

<script>

    import MatrixInput from "../components/MatrixInput.vue";

    function binaryOperate(api) {
        return async function () {
            let A = this.readMatrix(this.$refs.matrixA);
            let B = this.readMatrix(this.$refs.matrixB);
            let data = {
                type: A.type === "number" && B.type === "number" ? "number" : "string",
                mat1: A.matrix,
                mat2: B.matrix,
            }
            let resp = await fetch(api, {
                method: "post",
                body: JSON.stringify(data)
            });
            if (!resp.ok) {
                this.$message.error('Error: operation failed');
                return;
            }
            let json = await resp.json();
            this.result = json.data;
        }
    }

    export default {
        name: "Matrix",
        data: function () {
            return {
                result: []
            }
        },
        components: {
            MatrixInput
        },
        methods: {
            plus: binaryOperate("/api/plus"),
            minus: binaryOperate("/api/minus"),
            times: binaryOperate("/api/times"),

            readMatrix: function (root) {
                let rows = root.rows;
                let cols = root.cols;
                let isNumber = true;
                for (let i = 0; i < rows; i++) {
                    for (let j = 0; j < cols; j++) {
                        let cell = root.matrix[i][j].trim();
                        if (cell.length === 0) {
                            continue;
                        }
                        if (isNaN(cell)) {
                            isNumber = false;
                        }
                    }
                }

                let result = [];
                for (let i = 0; i < rows; i++) {
                    result.push([]);
                    for (let j = 0; j < cols; j++) {
                        if (isNumber) {
                            result[i].push(parseFloat(root.matrix[i][j]));
                        } else {
                            result[i].push(root.matrix[i][j]);
                        }
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
        }
    }
</script>

<style scoped>
    .operator-panel {
        width: 100%;
        height: 100%;
        display: flex;
        flex-direction: column;
        box-sizing: border-box;
        padding: 6em 2em 0;
    }
</style>