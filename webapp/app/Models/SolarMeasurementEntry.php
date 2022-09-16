<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class SolarMeasurementEntry extends Model
{
    use HasFactory;

    protected $table = 'rb_solar';

    protected $guarded = [
        'id',
        'timestamp',
        'wh_since_reset',
        'wh_total',
        'watts',
    ];
}
