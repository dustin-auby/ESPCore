/*
  ESPCore.h - Library for maintaining and viewing data comming from an esp device.
  Created by Dustin Auby, October 14, 2021.
  Released into the public domain.
*/

#ifndef ESPCoreContentDashboard_h
#define ESPCoreContentDashboard_h


const char _DASHBOARD_HTML[] PROGMEM = R"rawliteral(
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
            <v-container>
                <v-row>
                    <v-col cols="2" v-for="(obj, key, index) in getSwitches">
                        <v-switch
                            @change="switchAction(obj, key)"
                            v-model="obj.value"
                            :label="obj.name"
                            :color="obj.color"
                            :value="obj.name"
                            hide-details
                        ></v-switch>
                    </v-col>
                </v-row>
                <v-row>
                    <v-col cols="12" lg="2" sm="6" v-for="(obj, key, index) in getVars">
                        <v-card
                                class="mx-auto"
                                max-width="400"
                        >
                            <v-list-item two-line>
                                <v-list-item-content>
                                    <v-list-item-title class="text-h5">
                                        {{obj.name}}
                                    </v-list-item-title>
                                </v-list-item-content>
                            </v-list-item>

                            <v-card-text>
                                <v-row align="center">
                                    <v-col
                                            class="text-h3"
                                            cols="12"
                                    >
                                        {{obj.value}} {{obj.unit}}
                                    </v-col>
                                </v-row>
                            </v-card-text>

                        </v-card>
                    </v-col>
                </v-row>
            </v-container>
        </v-main>
    </v-app>
</div>
<script src="https://cdn.jsdelivr.net/npm/vue@2.x/dist/vue.js"></script>
<script src="https://cdn.jsdelivr.net/npm/vuetify@2.x/dist/vuetify.js"></script>
<script src="https://cdn.jsdelivr.net/npm/axios/dist/axios.min.js"></script>
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

                console.log(_this.json);
                Object.keys(_this.json).forEach(key => {
                    console.log(key, _this.json[key]);
                    if(_this.mapped_listeners.indexOf(key) === -1){
                        _this.mapped_listeners.push(key);
                        console.log("listener bound for " , key);
                        source.addEventListener(key, function(e) {
                        console.log("got value for " + key , e.data);
                        _this.json[key].value = e.data;
                        console.log(_this.json);
                        }, false);
                    }
                });

                Object.keys(_this.switches).forEach(key => {
                    console.log(key, _this.switches[key]);
                    if(_this.mapped_listeners.indexOf(key) === -1){
                        _this.mapped_listeners.push(key);
                        console.log("listener bound for " , key);
                        source.addEventListener(key, function(e) {
                        console.log("got value for " + key , e.data);
                        _this.switches[key].value = e.data;
                        console.log(_this.switches);
                        }, false);
                    }
                });

                source.addEventListener('rssi', function(e) {
                    _this.signal = e.data;
                }, false);

            }
        },
        data: () => ({
            projectName: "%PROJECT_TITLE%",
            drawer: false,
            drawer_open: false,
            test:0,
            content: '',
            signal: '%SIGNAL%',
            json: JSON.parse('%JSON_VARS%'),
            switches: JSON.parse('%JSON_SWITCHES%'),
            mapped_listeners: []
        }),
        computed:{
            getVars(){
                if(Object.keys(this.json).length > 0){
                    return this.json;
                }else { return []; }
            },
            getSwitches(){
                if(Object.keys(this.switches).length > 0){
                    return this.switches;
                }else { return []; }
            },
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
                let sigq = this.getSignalQuality();
                console.log("this.getSignalQuality()", sigq);
                switch(sigq){
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
            switchAction(obj, evt){
                console.log("obj", obj);
                console.log("evt", evt);
//                axios.post("/api/admin/reports/load-chart-data", {
//                    uuid: chart._uuid,
//                }).then((response) => {
//
//                }).catch((er) => {
//                    console.log(er);
//                });
            },
            getSignalQuality(){
                let signal_s = parseFloat(this.signal);
                if(isNaN(signal_s)){
                    signal_s = 0;
                }
                if(signal_s >=  -65){
                    return "good";
                }else if(signal_s >= -85 && signal_s < -65){
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