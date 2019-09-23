import {Routes, RouterModule} from '@angular/router';
import {HomeComponent} from './component/home/home.component';
import {StatsComponent} from './component/stats/stats.component';
import {ChartComponent} from './component/chart/chart.component';

export const AppRoutes: Routes = [
  {path: '', component: HomeComponent},
  {path: 'stats', component: StatsComponent},
  {path: 'chart', component: ChartComponent},
];


