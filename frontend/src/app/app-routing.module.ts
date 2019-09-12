import {NgModule} from '@angular/core';
import {Routes, RouterModule} from '@angular/router';
import {HomeComponent} from './component/home/home.component';
import {StatsComponent} from './component/stats/stats.component';
import {ChartComponent} from './component/chart/chart.component';

const routes: Routes = [
  {path: '', component: HomeComponent},
  {path: 'stats', component: StatsComponent},
  {path: 'chart', component: ChartComponent},
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})

export class AppRoutingModule {
}
