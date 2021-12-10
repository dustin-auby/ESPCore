/*
  ESPCore.h - Library for maintaining and viewing data comming from an esp device.
  Created by Dustin Auby, October 14, 2021.
  Released into the public domain.
*/

#ifndef ESPCoreContentUpdate_h
#define ESPCoreContentUpdate_h


const char _UPDATE_HTML[] PROGMEM = R"rawliteral(
<html>
<head>
    <title>%PROJECT_TITLE%</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://fonts.googleapis.com/css?family=Roboto:100,300,400,500,700,900" rel="stylesheet">
    <link href="https://cdn.jsdelivr.net/npm/vuetify@2.x/dist/vuetify.min.css" rel="stylesheet">
    <link href="https://cdn.jsdelivr.net/npm/@mdi/font@6.x/css/materialdesignicons.min.css" rel="stylesheet">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
</head>
<body style="background: #121212;">
<div id="app">
    <v-app id="inspire">
        <v-navigation-drawer class="sidemenu" :permanent="!$vuetify.breakpoint.xsOnly" app overflow clipped :mini-variant="!drawer && !$vuetify.breakpoint.xsOnly" mini-variant-width="60" v-model="drawer_open">
            <template >
                <div >
                    <v-list-item-group>

                        <template >
                            <v-tooltip right :disabled="drawer || $vuetify.breakpoint.xsOnly">
                                <template v-slot:activator="{ on }">
                                    <v-list-item v-on="on" href="./" exact  :class="isActive('/')">
                                        <v-list-item-icon>
                                            <v-icon>mdi-view-dashboard-outline</v-icon>
                                        </v-list-item-icon>
                                        <v-list-item-content>
                                            <v-list-item-title>Dashboard</v-list-item-title>
                                        </v-list-item-content>
                                    </v-list-item>
                                </template>
                                <span>Dashboard</span>
                            </v-tooltip>
                        </template>

                    </v-list-item-group>
                    <v-divider></v-divider>
                    <v-list>
                        <v-list-item-group>

                            <template >
                                <v-tooltip right :disabled="drawer || $vuetify.breakpoint.xsOnly">
                                    <template v-slot:activator="{ on }">
                                        <v-list-item v-on="on" href="update" exact  :class="isActive('update')">
                                            <v-list-item-icon>
                                                <v-icon>mdi-cellphone-arrow-down</v-icon>
                                            </v-list-item-icon>
                                            <v-list-item-content>
                                                <v-list-item-title>Update</v-list-item-title>
                                            </v-list-item-content>
                                        </v-list-item>

                                    </template>
                                    <span>Update</span>
                                </v-tooltip>

                                <v-tooltip right :disabled="drawer || $vuetify.breakpoint.xsOnly">
                                    <template v-slot:activator="{ on }">
                                        <v-list-item v-on="on" href="info" exact  :class="isActive('info')">
                                        <v-list-item-icon>
                                            <v-icon>mdi-tune-vertical</v-icon>
                                        </v-list-item-icon>
                                        <v-list-item-content>
                                            <v-list-item-title>Device Info</v-list-item-title>
                                        </v-list-item-content>
                                    </v-list-item>

                                    </template>
                                    <span>Device Info</span>
                                </v-tooltip>
                                <v-tooltip right :disabled="drawer || $vuetify.breakpoint.xsOnly">
                                    <template v-slot:activator="{ on }">
                                        <v-list-item v-on="on" href="logs" exact  :class="isActive('info')">
                                        <v-list-item-icon>
                                            <v-icon>mdi-console</v-icon>
                                        </v-list-item-icon>
                                        <v-list-item-content>
                                            <v-list-item-title>Device Logs</v-list-item-title>
                                        </v-list-item-content>
                                    </v-list-item>

                                    </template>
                                    <span>Device Logs</span>
                                </v-tooltip>
                            </template>

                        </v-list-item-group>

                    </v-list>
                </div>
            </template>
        </v-navigation-drawer>
        <v-app-bar app  clipped-left dark :height="($vuetify.breakpoint.xsOnly) ? 50 : 60">
            <v-app-bar-nav-icon @click.stop="toggle_nav"></v-app-bar-nav-icon>
            <v-toolbar-title>{{projectName}}</v-toolbar-title>
            <v-spacer></v-spacer>
            <v-tooltip bottom>
              <template v-slot:activator="{ on, attrs }">
                <v-btn 
                 v-bind="attrs"
                 v-on="on"
                 href="info"
                icon>
                    <v-icon :color="signalQuality">{{signalQualityIcon}}</v-icon>
                </v-btn>
              </template>
              <span>{{signal}} dBm</span>
            </v-tooltip>
        </v-app-bar>
        <v-main >
                <v-card class="ma-15 pa-5">

                    <form action="/update" method="POST" enctype='multipart/form-data'>
                        <v-radio-group
                                v-model="upload_type"
                                row
                        >
                            <v-radio
                                    label="Firmware"
                                    value="firmware"
                            ></v-radio>
                            <v-radio
                                    label="File system"
                                    value="filesystem"
                            ></v-radio>
                        </v-radio-group>
                        <v-file-input
                                label="File input"
                                name='update'
                        ></v-file-input>

                        <v-btn
                                class="mr-4"
                                type="submit"
                        >
                            UPDATE
                        </v-btn>
                    </form>
                </v-card>
        </v-main>
    </v-app>
</div>
<script src="https://cdn.jsdelivr.net/npm/vue@2.x/dist/vue.js"></script>
<script src="https://cdn.jsdelivr.net/npm/vuetify@2.x/dist/vuetify.js"></script>
<script>
    new Vue({
        el: '#app',
        vuetify: new Vuetify({
            theme: { dark: true },
        }),
        mounted() {
            let _this = this;
            if (!!window.EventSource) {
                var source = new EventSource('/events');

                source.addEventListener('open', function(e) {
                    console.log("Events Connected");
                }, false);
                source.addEventListener('error', function(e) {
                    if (e.target.readyState != EventSource.OPEN) {
                        console.log("Events Disconnected");
                    }
                }, false);

                source.addEventListener('message', function(e) {
                    console.log("message", e.data);
                }, false);

                source.addEventListener('temperature', function(e) {
                    console.log("temperature", e.data);
                }, false);

                source.addEventListener('humidity', function(e) {
                    console.log("humidity", e.data);
                }, false);

                source.addEventListener('pressure', function(e) {
                    console.log("pressure", e.data);
                    _this.test = e.data;
                }, false);
            }
        },
        data: () => ({
            projectName: "%PROJECT_TITLE%",
            drawer: false,
            drawer_open: false,
            test:0,
            content: '',
            upload_type: 'firmware',
            signal: '%SIGNAL%',
        }),
        computed:{
            signalQuality(){
                switch(this.getSignalQuality()){
                    case 'good':
                        return 'success';
                    case 'fair':
                        return 'success';
                    case 'poor':
                        return 'warning';
                    case 'none':
                        return 'error';
                }
                return 'error';
            },
            signalQualityIcon(){
                console.log("this.getSignalQuality()", this.getSignalQuality());
                switch(this.getSignalQuality()){
                    case 'good':
                        return 'mdi-wifi-strength-4';
                    case 'fair':
                        return 'mdi-wifi-strength-3';
                    case 'poor':
                        return 'mdi-wifi-strength-1-alert';
                    case 'none':
                        return 'mdi-wifi-strength-alert-outline';
                }
                return 'mdi-wifi-strength-off-outline';
            }
        },
        methods: {
            getSignalQuality(){
                let signal_s = parseFloat(this.signal);
                if(isNaN(signal_s)){
                    signal_s = 0;
                }
                if(signal_s >=  -65){
                    return "good";
                }else if(signal_s >= -85 && signal_s < -75){
                    return "fair";
                }else if(signal_s >= -95 && signal_s < -85){
                    return "poor";
                }else if(signal_s >= -95 && signal_s < -85 || signal_s === 0){
                    return "none";
                }
            },
            toggle_nav(){
                if(this.$vuetify.breakpoint.xsOnly){
                    this.drawer = false;
                    this.drawer_open = !this.drawer_open;
                }else{
                    this.drawer_open = false;
                    this.drawer = !this.drawer;
                }
            },
            isActive(to){
            },
        }
    })
</script>
<script>
</script>
</body>
</html>
)rawliteral";
#endif