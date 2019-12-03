import {BrowserModule} from '@angular/platform-browser';
import {NgModule} from '@angular/core';
import { ClickOutsideModule } from 'ng-click-outside';
import {MatCardModule} from '@angular/material/card';
import { RouterModule } from '@angular/router';
import {AppRoutes} from './app.routes';
import {AppComponent} from './app.component';
import {StatsComponent} from './component/stats/stats.component';
import {BrowserAnimationsModule} from '@angular/platform-browser/animations';
import {MatButtonModule} from '@angular/material/button';

import {MatSidenavModule} from '@angular/material/sidenav';
import {MatToolbarModule} from '@angular/material/toolbar';
import {MatListModule} from '@angular/material/list';

import {MatIconModule} from '@angular/material/icon';

import {HttpClientModule} from '@angular/common/http';
import { SidenavComponent } from './component/sidenav/sidenav.component';
import { ToolbarComponent } from './component/toolbar/toolbar.component';
import { HomeComponent } from './component/home/home.component';
import { ChartsModule } from 'ng2-charts';
import {ChartComponent} from './component/chart/chart.component';
import {ApiService} from './service/api/api.service';
import {MatTooltipModule} from "@angular/material/tooltip";
import {MatTableModule} from "@angular/material/table";
import {MatSliderModule} from "@angular/material/slider";
import {MatFormFieldModule} from "@angular/material/form-field";
import {MatInputModule} from "@angular/material/input";
import {FormsModule} from "@angular/forms";
import {MatOptionModule} from "@angular/material/core";
import {MatSelectModule} from "@angular/material/select";
import {MatTabsModule} from "@angular/material/tabs";
@NgModule({
  declarations: [
    AppComponent,
    StatsComponent,
    SidenavComponent,
    ToolbarComponent,
    HomeComponent,
    ChartComponent
  ],
  imports: [
    MatInputModule,
    BrowserModule,
    MatSidenavModule,
    MatIconModule,
    BrowserAnimationsModule,
    HttpClientModule,
    MatToolbarModule,
    MatListModule,
    MatButtonModule,
    MatCardModule,
    ClickOutsideModule,
    ChartsModule,
    RouterModule.forRoot(AppRoutes),
    MatTooltipModule,
    MatTableModule,
    MatSliderModule,
    MatFormFieldModule,
    FormsModule,
    MatOptionModule,
    MatSelectModule,
    MatTabsModule
  ],
  providers: [ApiService],
  bootstrap: [AppComponent]
})
export class AppModule {
}
