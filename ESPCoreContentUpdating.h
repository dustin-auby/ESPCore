/*
  ESPCore.h - Library for maintaining and viewing data comming from an esp device.
  Created by Dustin Auby, October 14, 2021.
  Released into the public domain.
*/

#ifndef ESPCoreContentUpdating_h
#define ESPCoreContentUpdating_h


const char _UPDATING_HTML[] PROGMEM = R"rawliteral(
<html>
<head>
    <title>%PROJECT_TITLE%</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://fonts.googleapis.com/css?family=Roboto:100,300,400,500,700,900" rel="stylesheet">
    <link href="https://cdn.jsdelivr.net/npm/vuetify@2.x/dist/vuetify.min.css" rel="stylesheet">
    <link href="https://cdn.jsdelivr.net/npm/@mdi/font@6.x/css/materialdesignicons.min.css" rel="stylesheet">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
</head>
<body>
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

        </v-app-bar>
        <v-main >
            <v-card class="d-flex justify-space-around"
                flat
                style="background-color: transparent;"
            >
                <v-card
                        class="ma-15 pa-5"
                        max-width="600"
                        width="500"
                >
                    <v-toolbar
                            dense
                            flat
                    >
                        <v-toolbar-title class="grey--text">
                            {{getUpdateStage}}
                        </v-toolbar-title>

                        <v-spacer></v-spacer>
                        <v-progress-circular
                                v-if="!updated"
                                indeterminate
                                color="primary"
                        ></v-progress-circular>
                    </v-toolbar>
                    <v-card-text class="py-0">
                        <v-timeline
                                align-top
                                dense
                        >
                            <v-timeline-item
                                    color="teal lighten-3"
                                    small
                            >
                                <v-row class="pt-1">
                                    <v-col cols="6">
                                        <strong>Firmware uploaded</strong>
                                    </v-col>
                                </v-row>
                            </v-timeline-item>

                            <v-timeline-item
                                    color="teal lighten-3"
                                    small
                            >
                                <v-row class="pt-1">
                                    <v-col cols="6">
                                        <strong>Update successful</strong>
                                    </v-col>
                                </v-row>
                            </v-timeline-item>

                            <v-timeline-item
                                    :color="updated ? 'teal lighten-3' : 'pink'"
                                    small
                            >
                                <v-row class="pt-1">
                                    <v-col cols="6">
                                        <strong>Restarting device</strong>
                                    </v-col>
                                </v-row>
                            </v-timeline-item>

                            <v-timeline-item
                                    :color="updated ? 'pink' : 'teal lighten-3'"
                                    small
                            >
                                <v-row class="pt-1">
                                    <v-col cols="6">
                                        <strong>Update complete</strong>
                                        <v-icon
                                                v-if="updated"
                                                large
                                                color="green darken-2"
                                        >
                                            mdi-check
                                        </v-icon>
                                    </v-col>
                                </v-row>
                            </v-timeline-item>
                        </v-timeline>
                    </v-card-text>
                </v-card>
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
            setTimeout(async function () {
                await _this.checkIfUpdated();
            }, 2000);
        },
        data: () => ({
            projectName: "%PROJECT_TITLE%",
            drawer: false,
            drawer_open: false,
            signal: '%SIGNAL%',
            updated: false,
        }),
        computed:{
            getUpdateStage(){
                if(this.updated){
                    return "Updated successfully";
                }else{
                    return "Rebooting device please wait ....";
                }
            },
        },
        methods: {
            async checkIfUpdated(){
                let _this = this;
                    setTimeout(async function () {
                        console.log("Checking update state");
                        try {
                            const response = await axios.get('/ping');
                            _this.updated = true;
                        } catch (error) {
                            console.error(error);
                            _this.checkIfUpdated();
                        }
                    }, 1000);
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