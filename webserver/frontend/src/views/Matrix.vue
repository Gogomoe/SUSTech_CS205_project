<template>
    <div>
        <h1>Matrix Calculate</h1>
        <a-row>
            <a-col :span="10">
                <h3>Matrix A</h3>
                <MatrixInput ref="matrixA" v-on:update:result="result = $event"></MatrixInput>
            </a-col>
            <a-col :span="4">
                <div class="operator-panel">
                    <a-button v-on:click="exchange">↔</a-button>
                    <a-button v-on:click="plus">A + B</a-button>
                    <a-button v-on:click="minus">A - B</a-button>
                    <a-button v-on:click="cross_times">A × B</a-button>
                    <a-button v-on:click="inner_times">A · B</a-button>
                </div>
            </a-col>
            <a-col :span="10">
                <h3>Matrix B</h3>
                <MatrixInput ref="matrixB" v-on:update:result="result = $event"></MatrixInput>
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
            let A = this.$refs.matrixA.readMatrix();
            let B = this.$refs.matrixB.readMatrix();
            let isNumber = A.type === "number" && B.type === "number";
            if (isNumber) {
                A.matrix.data = A.matrix.data.map(row => row.map(it => parseFloat(it)));
                B.matrix.data = B.matrix.data.map(row => row.map(it => parseFloat(it)));
            }
            let data = {
                type: isNumber ? "number" : "string",
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
            cross_times: binaryOperate("/api/cross_times"),
            inner_times: binaryOperate("/api/inner_times"),
            exchange: function () {
                let A = this.$refs.matrixA.readMatrix().matrix;
                let B = this.$refs.matrixB.readMatrix().matrix;
                let rows = Math.max(A.rows, B.rows);
                let cols = Math.max(A.cols, B.cols);
                for (let i = 0; i < rows; i++) {
                    if (A.data.length <= i) {
                        A.data.push([]);
                    }
                    if (B.data.length <= i) {
                        B.data.push([]);
                    }
                    for (let j = 0; j < cols; j++) {
                        if (A.data[i].length <= i) {
                            A.data[i].push("");
                        }
                        if (B.data[i].length <= i) {
                            B.data[i].push("");
                        }
                    }
                }
                this.$refs.matrixA.rows = B.rows;
                this.$refs.matrixA.cols = B.cols;
                this.$refs.matrixA.matrix = B.data;

                this.$refs.matrixB.rows = A.rows;
                this.$refs.matrixB.cols = A.cols;
                this.$refs.matrixB.matrix = A.data;
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