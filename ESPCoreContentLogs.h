/*
  ESPCore.h - Library for maintaining and viewing data comming from an esp device.
  Created by Dustin Auby, October 14, 2021.
  Released into the public domain.
*/

#ifndef ESPCoreContentLogs_h
#define ESPCoreContentLogs_h


const char _LOGS_HTML[] PROGMEM = R"rawliteral(
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
                <v-toolbar
                        dense
                        flat
                >
                    <v-text-field
                            v-model="search"
                            prepend-icon="mdi-magnify"
                            label="Search"
                            single-line
                            hide-details
                            clearable

                            dense
                    ></v-text-field>
                    <v-spacer></v-spacer>

                    <v-btn @click="auto_page = !auto_page" icon>
                        <v-icon>{{auto_page ? 'mdi-pause' : 'mdi-play'}}</v-icon>
                    </v-btn>
                    <v-btn @click="clearLogs" icon>
                        <v-icon>mdi-delete-outline</v-icon>
                    </v-btn>
                    <v-btn icon>
                        <v-icon>mdi-dots-vertical</v-icon>
                    </v-btn>
                </v-toolbar>
                <v-data-table
                        class="text--primary"
                        :headers="headers"
                        dense
                        dark
                        :items="log_objects"
                        :search="search"
                        :items-per-page="itemsPerPage"
                        :rowsPerPageItems="itemsPerPageArray"
                        sort-by="datetime"
                        sort-asc
                        :page="current_page"
                        :footer-props="{
                        'items-per-page-options': [20, 50, 100, 200, 500, -1]
                        }"
                >
                    <template v-slot:item.datetime="{ item }">
                        <span class="text--primary">{{item.datetime}}</span>
                    </template>
                    <template v-slot:item.text="{ item }">
                        <span :class="getTextColor(item.type)">{{item.text}}</span>
                    </template>
                    <template v-slot:item.type="{ item }">
                        <v-chip
                                :color="getColor(item.type)"
                                dark
                                x-small
                        >
                            {{ getType(item.type) }}
                        </v-chip>
                    </template>
                </v-data-table>
                <v-text-field
                v-model="command"
                solo
                label="Send Command"
                prepend-icon="mdi-console"
                append-outer-icon="mdi-send"
                @click:append-outer="sendCommand"
                :loading="command_sending"
                ></v-text-field>
            </v-card>
        </v-main>
    </v-app>
</div>
<script src="https://cdn.jsdelivr.net/npm/vue@2.x/dist/vue.js"></script>
<script src="https://cdn.jsdelivr.net/npm/vuetify@2.x/dist/vuetify.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/moment.js/2.29.1/moment.min.js" integrity="sha512-qTXRIMyZIFb8iQcfjXWCO8+M5Tbc38Qi5WzdPOYZHIlZpzBHG3L3by84BBBOiRGiEb7KKtAOAs5qYdUiZiQNNQ==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
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

                source.addEventListener('logger', function(e) {
                    let json = JSON.parse(e.data);
                    _this.log_objects.push({
                        text:json.message,
                        type: json.type,
                        datetime: moment().format("YYYY-MM-DD HH:mm:ss")
                    });

                    if(_this.auto_page){
                        _this.current_page = _this.getPagesTotal();
                        console.log( _this.current_page);
                    }
                    //_this.logs = _this.logs + "\n" + e.data;
                }, false);

                source.addEventListener('ssid', function(e) {
                    _this.wifi_ssid = e.data;
                }, false);

                source.addEventListener('rssi', function(e) {
                    _this.signal = e.data;
                }, false);
            }
        },
        data: () => ({
            projectName: "%PROJECT_TITLE%",
            drawer: false,
            auto_page: true,
            drawer_open: false,
            logs: '',
            signal: '%SIGNAL%',
            mapped_listeners: [],
            log_objects:[],
            search: '',
            itemsPerPageArray: [20, 50, 100, 200, 500],
            itemsPerPage: 20,
            current_page: 1,
            command: "",
            command_sending:false,
            headers: [
                {
                    text: 'Time',
                    align: 'start',
                    filterable: false,
                    value: 'datetime',
                    width: 150
                },
                { text: 'Log', value: 'text' },
                { text: 'Type', value: 'type', width: 90 },
            ],
        }),
        computed:{

            getProps(){
                if(Object.keys(this.json).length > 0){
                    return this.json;
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
            getPagesTotal(){
                let pages = parseInt(this.log_objects.length / this.itemsPerPage);
                console.log("this.itemsPerPage", this.itemsPerPage);
                console.log("this.log_objects.length", this.log_objects.length);
                console.log("pages", pages);
                if(this.log_objects.length % this.itemsPerPage > 0){
                    pages++;
                }
                console.log("pages2", pages);
                return  pages;
            },
            clearLogs(){
                this.log_objects = [];
            },
            getColor (type) {
                switch(type){
                    case 'warn':
                        return '#FFB74D';
                    case 'error':
                        return '#EF5350';
                    default:
                        return 'grey';
                }
            },
            getTextColor (type) {
                switch(type){
                    case 'warn':
                        return 'orange--text text--lighten-2';
                    case 'error':
                        return 'red--text text--lighten-1';
                    default:
                        return 'text--primary';
                }
            },
            getType (type) {
                switch(type){
                    case 'warn':
                        return 'warning';
                    case 'error':
                        return 'error';
                    default:
                        return 'Debug';
                }
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
            sendCommand(){
                this.command_sending = true;
                let _this = this;

                const params = new URLSearchParams();
                params.append('c', this.command);

                axios.post('/command', params)
                  .then(function (response) {
                    console.log(response);
                    _this.command_sending = false;
                    _this.command = "";
                  })
                  .catch(function (error) {
                    console.log(error);
                    _this.command_sending = false;
                  });
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