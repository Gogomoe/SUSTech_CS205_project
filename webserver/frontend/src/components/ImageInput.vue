<template>
    <div>
        <a-upload
                list-type="picture"
                :file-list="fileList"
                :remove="handleRemove"
                :before-upload="beforeUpload"
        >
            <div v-if="fileList.length === 0">
                <a-button>
                    <a-icon type="upload"/>
                    upload
                </a-button>
            </div>
        </a-upload>

    </div>
</template>

<script>

    export default {
        name: "ImageInput",
        data() {
            return {
                fileList: [],
            };
        },
        methods: {
            handleRemove() {
                this.fileList = [];
            },
            beforeUpload(file) {
                let fileItem = {
                    uid: `x${file.uid}`,
                    name: file.name,
                    status: "uploading",
                    origin: file
                };
                this.fileList = [fileItem];

                let reader = new FileReader();
                reader.onload = function () {
                    fileItem.url = 'data:image/jpeg;base64,' + btoa(
                        new Uint8Array(reader.result)
                            .reduce((data, byte) => data + String.fromCharCode(byte), '')
                    );

                    fileItem.bytes = reader.result;

                    let img = new Image();
                    img.src = fileItem.url;
                    img.onload = function () {
                        fileItem.width = this.width;
                        fileItem.height = this.height;
                        fileItem.status = "done";
                    }
                }
                reader.readAsArrayBuffer(file);

                return false;
            },
            readImage() {
                if (this.fileList.length === 0 || this.fileList[0].status !== "done") {
                    return null;
                }
                return this.fileList[0];
            }
        },
    }
</script>

<style scoped>

</style>