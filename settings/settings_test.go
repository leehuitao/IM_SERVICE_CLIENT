package settings

import "testing"

func TestInitConfiguration(t *testing.T) {
	var tests []struct {
		name    string
		wantErr bool
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if err := InitConfiguration(); (err != nil) != tt.wantErr {
				t.Errorf("InitConfiguration() error = %v, wantErr %v", err, tt.wantErr)
			}
		})
	}
}
