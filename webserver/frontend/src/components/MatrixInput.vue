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
    </div>
</template>

<script>
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