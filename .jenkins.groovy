pipeline {
    agent any

    environment {
        PLX_SDK_DIR = "${WORKSPACE}/broadcom_pci_pcie_sdk"
    }

    stages {
        stage('PLX') {
            steps{
                checkout([$class: 'GitSCM', branches: [[name: '*/master']], extensions: [[$class: 'RelativeTargetDirectory', relativeTargetDir: 'broadcom_pci_pcie_sdk']], userRemoteConfigs: [[credentialsId: 'jenkins-git-xia-com', url: 'http://172.31.20.204:8081/externals/broadcom_pci_pcie_sdk.git']]])
                sh '''
                make -C ${PLX_SDK_DIR}/PlxApi/ PLX_NO_CLEAR_SCREEN=1
                if [ ! -f ${PLX_SDK_DIR}/PlxApi/Library/PlxApi.a ];then echo "PLX Static lib missing!"; exit 1; fi
                if [ ! -f ${PLX_SDK_DIR}/PlxApi/Library/PlxApi.so ];then echo "PLX SO lib missing!"; exit 1; fi
                '''
            }
        }
        stage('Check workspace') {
            steps {
                sh '''ls'''
            }
        }
        stage('PixieSDK') {
            steps{
                dir('build') {
                    sh '''
                    cmake3 ../
                    make
                    ./tests/unit/legacy/legacy_unit_test_runner
                    '''
                }
            }
        }
    }
    post {
        always {
            cleanWs()
        }
    }
}
